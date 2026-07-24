package com.zeroone.star.configuration.service.impl;

import com.baomidou.mybatisplus.core.conditions.query.QueryWrapper;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.configuration.entity.Metadata;
import com.zeroone.star.configuration.mapper.MetadataMapper;
import com.zeroone.star.configuration.mapper.MsMetadataMapper;
import com.zeroone.star.configuration.service.MetadataService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j5.configuration.MetadataDetailDTO;
import com.zeroone.star.project.dto.j5.configuration.MetadataListDTO;
import com.zeroone.star.project.query.j5.configuration.MetadataQuery;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
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
     *
     * @return 启用状态的字段名称列表，按 sort_order 升序排列；无数据时返回空列表
     */
    @Override
    public List<String> listFieldNames() {
        QueryWrapper<Metadata> queryWrapper = new QueryWrapper<>();
        queryWrapper.select("label");
        queryWrapper.eq("is_enabled", 1);
        queryWrapper.orderByAsc("sort_order");
        List<Metadata> fields = baseMapper.selectList(queryWrapper);
        return fields.stream()
                .map(Metadata::getLabel)
                .collect(Collectors.toList());
    }

    /**
     * 分页查询元数据字段列表。
     * <p>
     * 支持按关键词（模糊匹配 field_key 或 label）、取值来源（精确匹配 value_source）
     * 以及状态（is_enabled）进行筛选。若未指定状态则默认查询启用状态的字段。
     * 结果按 sort_order 升序排列。
     * </p>
     *
     * @param query 分页查询条件，包含页码、每页大小、关键词、取值来源和状态筛选条件
     * @return 分页包装的元数据字段列表 DTO
     */
    @Override
    public PageDTO<MetadataListDTO> listFields(MetadataQuery query) {
        Page<Metadata> page = new Page<>(query.getPageIndex(), query.getPageSize());
        QueryWrapper<Metadata> queryWrapper = new QueryWrapper<>();

        // 状态筛选：优先使用查询参数，未指定时默认查询启用状态
        if (StringUtils.hasText(query.getStatus())) {
            queryWrapper.eq("is_enabled", Integer.parseInt(query.getStatus()));
        } else {
            queryWrapper.eq("is_enabled", 1);
        }

        // 关键词模糊搜索：匹配 field_key 或 label
        if (StringUtils.hasText(query.getKeyword())) {
            queryWrapper.and(wrapper -> wrapper
                    .like("field_key", query.getKeyword())
                    .or()
                    .like("label", query.getKeyword()));
        }

        // 取值来源精确匹配
        if (StringUtils.hasText(query.getValueSource())) {
            queryWrapper.eq("value_source", query.getValueSource());
        }

        queryWrapper.orderByAsc("sort_order");
        Page<Metadata> fieldPage = baseMapper.selectPage(page, queryWrapper);
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
