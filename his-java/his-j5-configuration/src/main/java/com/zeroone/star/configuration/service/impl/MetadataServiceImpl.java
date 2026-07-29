package com.zeroone.star.configuration.service.impl;

import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.configuration.entity.Metadata;
import com.zeroone.star.configuration.mapper.MetadataMapper;
import com.zeroone.star.configuration.mapper.MsMetadataMapper;
import com.zeroone.star.configuration.service.MetadataService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j5.Metadata.MetadataDetailDTO;
import com.zeroone.star.project.dto.j5.Metadata.MetadataListDTO;
import com.zeroone.star.project.query.j5.Metadata.MetadataQuery;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.stereotype.Service;
import org.springframework.util.StringUtils;

import java.util.List;
import java.util.stream.Collectors;

/**
 * 元数据字段服务实现类。
 * <p>
 * 基于 MyBatis-Plus 的 {@link ServiceImpl} 提供对 {@link Metadata} 实体的增删改查能力，
 * 实现 {@link MetadataService} 接口中定义的元数据字段查询业务逻辑。
 * 操作的表为 {@code emr_metadata_field_def}。
 * </p>
 *
 * <h3>SQL 性能优化说明</h3>
 * <p>原实现使用 {@code QueryWrapper} 搭配 {@code OR} 条件进行关键词搜索：</p>
 * <pre>{@code
 *   queryWrapper.and(w -> w.like("field_key", keyword).or().like("label", keyword));
 * }</pre>
 * <p>这会生成 {@code (field_key LIKE '%keyword%' OR label LIKE '%keyword%')}，
 * MySQL 优化器对跨列 OR + 前导通配符 LIKE 难以生成高效执行计划，通常退化为全表扫描。</p>
 *
 * <p>优化方案采用 <b>双路径策略</b>：</p>
 * <ul>
 *   <li><b>无关键词</b>：使用 {@link LambdaQueryWrapper} 标准分页，等值过滤利用索引高效定位</li>
 *   <li><b>有关键词</b>：走 {@link MetadataMapper#selectPageWithKeyword} 的 UNION ALL 查询，
 *       将 field_key 和 label 的匹配拆分为两个独立子查询，各自使用对应列的索引，
 *       避免 OR 导致的索引失效</li>
 * </ul>
 */
@Service
public class MetadataServiceImpl extends ServiceImpl<MetadataMapper, Metadata> implements MetadataService {

    @Autowired
    @Qualifier("msMetadataMapperImpl")
    MsMetadataMapper msMapper;

    /**
     * 查询所有启用状态的元数据字段名称（label）。
     * <p>
     * 仅查询状态为启用（is_enabled=1）的字段，按排序字段升序排列后提取字段显示名称。
     * 用于病历模板设计时展示可选元数据字段名称。
     * </p>
     * <p>
     * 性能优化：使用 {@link LambdaQueryWrapper#select} 仅查询 label 列，减少数据传输量。
     * MyBatis-Plus 的 {@code @TableLogic} 自动追加 is_deleted=0 条件。
     * </p>
     *
     * @return 启用状态的字段名称列表，按 sort_order 升序排列；无数据时返回空列表
     */
    @Override
    public List<String> listFieldNames() {
        LambdaQueryWrapper<Metadata> wrapper = new LambdaQueryWrapper<>();
        wrapper.select(Metadata::getLabel);
        wrapper.eq(Metadata::getIsEnabled, 1);
        wrapper.orderByAsc(Metadata::getSortOrder);
        List<Metadata> fields = baseMapper.selectList(wrapper);
        return fields.stream()
                .map(Metadata::getLabel)
                .collect(Collectors.toList());
    }

    /**
     * 分页查询元数据字段列表。
     * <p>
     * 支持按关键词（模糊匹配 field_key 或 label）、取值来源（精确匹配 value_source）
     * 以及状态（is_enabled）进行筛选。若未指定状态则默认查询启用状态的字段。
     * </p>
     *
     * <h3>SQL 性能优化</h3>
     * <p>采用双路径策略避免 OR 条件导致的全表扫描：</p>
     * <ul>
     *   <li><b>无关键词时</b>：使用 {@link LambdaQueryWrapper} 标准分页，简单等值过滤</li>
     *   <li><b>有关键词时</b>：使用 {@link MetadataMapper#selectPageWithKeyword}
     *       的 UNION ALL 查询，将跨列 OR 拆分为两个独立子查询</li>
     * </ul>
     *
     * @param query 分页查询条件，包含页码、每页大小、关键词、取值来源和状态筛选条件
     * @return 分页包装的元数据字段列表 DTO
     */
    @Override
    public PageDTO<MetadataListDTO> listFields(MetadataQuery query) {
        // 解析启用状态（默认 1 = 启用）
        Integer isEnabled = 1;
        if (StringUtils.hasText(query.getStatus())) {
            isEnabled = Integer.parseInt(query.getStatus());
        }

        Page<Metadata> page = new Page<>(query.getPageIndex(), query.getPageSize());
        Page<Metadata> fieldPage;

        if (StringUtils.hasText(query.getKeyword())) {
            // === 关键词路径：UNION ALL 策略 ===
            // 将 field_key 和 label 的模糊匹配拆分为两个独立子查询，
            // 各自使用对应列的索引，避免跨列 OR 导致的索引失效
            String valueSource = StringUtils.hasText(query.getValueSource())
                    ? query.getValueSource() : null;

            // 禁用 MyBatis-Plus 自动 count（复杂 UNION 查询需手动计数）
            page.setSearchCount(false);
            Long total = baseMapper.selectCountWithKeyword(isEnabled, valueSource, query.getKeyword());
            page.setTotal(total);

            if (total == 0) {
                // 无匹配记录，直接返回空页
                fieldPage = page;
            } else {
                fieldPage = baseMapper.selectPageWithKeyword(page, isEnabled, valueSource, query.getKeyword());
            }
        } else {
            // === 无关键词路径：MyBatis-Plus 标准分页 ===
            // 等值过滤，MyBatis-Plus 自动处理 count + LIMIT
            // 显式指定 SELECT 列，仅查询 MetadataListDTO 所需字段，避免 SELECT * 冗余传输
            LambdaQueryWrapper<Metadata> wrapper = new LambdaQueryWrapper<>();
            wrapper.select(Metadata::getId, Metadata::getFieldKey, Metadata::getLabel,
                           Metadata::getEditableDefault, Metadata::getValueSource);
            wrapper.eq(Metadata::getIsEnabled, isEnabled);
            if (StringUtils.hasText(query.getValueSource())) {
                wrapper.eq(Metadata::getValueSource, query.getValueSource());
            }
            wrapper.orderByAsc(Metadata::getSortOrder);
            fieldPage = baseMapper.selectPage(page, wrapper);
        }

        return PageDTO.create(fieldPage, msMapper::metadataFieldToListDTO);
    }

    /**
     * 根据主键 ID 获取单个元数据字段的详细信息。
     *
     * @param id 元数据字段主键 ID，不能为空
     * @return 元数据字段详情 DTO，包含完整的字段元信息
     * @throws IllegalArgumentException 当 id 为空时抛出
     * @throws RuntimeException        当对应字段不存在时抛出
     */
    @Override
    public MetadataDetailDTO getFieldDetail(String id) {
        if (!StringUtils.hasText(id)) {
            throw new IllegalArgumentException("字段ID不能为空");
        }
        Metadata field = baseMapper.selectById(id);
        if (field != null) {
            return msMapper.metadataFieldToDetailDTO(field);
        }
        throw new RuntimeException("字段不存在");
    }
}
