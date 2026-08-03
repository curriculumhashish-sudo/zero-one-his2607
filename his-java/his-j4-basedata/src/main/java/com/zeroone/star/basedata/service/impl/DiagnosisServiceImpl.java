package com.zeroone.star.basedata.service.impl;

import cn.hutool.core.bean.BeanUtil;
import cn.hutool.core.util.StrUtil;
import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.core.conditions.update.LambdaUpdateWrapper;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.basedata.entity.diagnosis.ClnConditionDefinitionDO;
import com.zeroone.star.basedata.mapper.DiagnosisMapper;
import com.zeroone.star.basedata.service.IDiagnosisService;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j4.diagnosis.DiagnosisSaveDTO;
import com.zeroone.star.project.dto.j4.diagnosis.DiagnosisUpdateDTO;
import com.zeroone.star.project.dto.j4.diagnosis.DiagnosisIdsDTO;
import com.zeroone.star.project.dto.j4.diagnosis.DiagnosisStatusDTO;
import com.zeroone.star.project.query.j4.diagnosis.DiagnosisDefinitionQuery;
import com.zeroone.star.project.query.j4.diagnosis.DiagnosisPageQuery;
import com.zeroone.star.project.query.j4.diagnosis.DiagnosisSelectPageQuery;
import com.zeroone.star.project.vo.j4.diagnosis.DiagnosisDefinitionDetailVO;
import com.zeroone.star.project.vo.j4.diagnosis.DiagnosisDetailVO;
import com.zeroone.star.project.vo.j4.diagnosis.DiagnosisListVO;
import com.zeroone.star.project.vo.j4.diagnosis.DiagnosisOptionVO;

import jakarta.annotation.Resource;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.time.LocalDateTime;
import java.util.List;

/**
 * <p>
 * 描述：诊断服务实现
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@Service
public class DiagnosisServiceImpl extends ServiceImpl<DiagnosisMapper, ClnConditionDefinitionDO> 
    implements IDiagnosisService {

    @Resource
    private MsDiagnosisMapper msDiagnosisMapper;

    @Resource
    private UserHolder userHolder;

    @Override
    public Page<ClnConditionDefinitionDO> pageQuery(DiagnosisDefinitionQuery query) {
        Page<ClnConditionDefinitionDO> page = new Page<>(query.getPageIndex(), query.getPageSize());
        LambdaQueryWrapper<ClnConditionDefinitionDO> wrapper = new LambdaQueryWrapper<>();
        wrapper.eq(ClnConditionDefinitionDO::getTenantId, userHolder.getCurrentTenantId());
        
        if (StrUtil.isNotBlank(query.getName())) {
            wrapper.like(ClnConditionDefinitionDO::getName, query.getName());
        }
        if (StrUtil.isNotBlank(query.getPyStr())) {
            wrapper.like(ClnConditionDefinitionDO::getPyStr, query.getPyStr());
        }
        if (StrUtil.isNotBlank(query.getWbStr())) {
            wrapper.like(ClnConditionDefinitionDO::getWbStr, query.getWbStr());
        }
        
        return baseMapper.selectPage(page, wrapper);
    }

    @Override
    public ClnConditionDefinitionDO getById(String id) {
        return baseMapper.selectById(id);
    }

    @Override
    public List<DiagnosisDefinitionDetailVO.ReferenceDetail> getReferences(String id) {
        // 暂时返回空列表，实际业务中需要实现引用关系查询逻辑
        return List.of();
    }

    @Override
    @Transactional
    public String saveDefinition(DiagnosisSaveDTO dto) {
        // 1. DTO转实体
        ClnConditionDefinitionDO entity = new ClnConditionDefinitionDO();
        BeanUtil.copyProperties(dto, entity);
        
        // 生成病情编码（格式：CDF + 8位数字序号）
        entity.setConditionCode(generateConditionCode());
        
        // 设置创建时间
        entity.setCreatedAt(LocalDateTime.now());
        entity.setUpdatedAt(LocalDateTime.now());
        
        // 设置租户ID
        entity.setTenantId(userHolder.getCurrentTenantId());
        
        // 2. 保存到数据库
        baseMapper.insert(entity);
        
        // 3. 返回新增记录ID
        return entity.getId();
    }
    
    /**
     * 生成病情编码，格式为 CDF + 8位数字
     */
    private String generateConditionCode() {
        // 简单实现：使用当前时间戳的一部分作为唯一编码
        // 实际项目中可能需要更复杂的逻辑来确保全局唯一性
        return "CDF" + System.currentTimeMillis() % 100000000L;
    }

    @Override
    @Transactional
    public void modifyDefinition(DiagnosisUpdateDTO dto) {
        // 1. 检查记录是否存在
        ClnConditionDefinitionDO existing = baseMapper.selectById(dto.getId());
        if (existing == null) {
            throw new RuntimeException("诊断记录不存在");
        }
        // 2. DTO转实体
        ClnConditionDefinitionDO entity = new ClnConditionDefinitionDO();
        BeanUtil.copyProperties(dto, entity);
        // 3. 保留系统字段
        entity.setCreatedAt(existing.getCreatedAt());
        entity.setCreatedBy(existing.getCreatedBy());
        entity.setIsDeleted(existing.getIsDeleted());
        entity.setDeletedAt(existing.getDeletedAt());
        entity.setDeletedBy(existing.getDeletedBy());
        entity.setVersion(existing.getVersion());
        entity.setConditionCode(existing.getConditionCode()); // 保留原编码
        entity.setTenantId(existing.getTenantId()); // 保留租户ID
        entity.setUpdatedAt(LocalDateTime.now()); // 更新时间
        // 4. 更新数据库
        baseMapper.updateById(entity);
    }

    @Override
    @Transactional
    public void modifyBatchStatus(DiagnosisStatusDTO dto) {
        // 1. 检查ID列表是否为空
        if (dto.getIds() == null || dto.getIds().isEmpty()) {
            throw new IllegalArgumentException("诊断ID列表不能为空");
        }
        // 2. 检查状态值是否有效
        if (!"ACTIVE".equals(dto.getStatus()) && !"INACTIVE".equals(dto.getStatus())) {
            throw new IllegalArgumentException("状态值必须为ACTIVE或INACTIVE");
        }
        // 3. 批量更新状态（限定租户）
        LambdaUpdateWrapper<ClnConditionDefinitionDO> updateWrapper = new LambdaUpdateWrapper<>();
        updateWrapper.eq(ClnConditionDefinitionDO::getTenantId, userHolder.getCurrentTenantId())
                .in(ClnConditionDefinitionDO::getId, dto.getIds())
                .set(ClnConditionDefinitionDO::getStatusEnum, dto.getStatus());
        int rowsAffected = baseMapper.update(null, updateWrapper);
        
        if (rowsAffected != dto.getIds().size()) {
            throw new RuntimeException("部分诊断记录更新失败");
        }
    }

    @Override
    @Transactional
    public void removeBatch(DiagnosisIdsDTO dto) {
        // 1. 检查ID列表是否为空
        if (dto.getIds() == null || dto.getIds().isEmpty()) {
            throw new IllegalArgumentException("诊断ID列表不能为空");
        }
        // 2. 逻辑删除（实体有@TableLogic注解，会自动转为update操作）
        List<String> ids = dto.getIds();
        String tenantId = userHolder.getCurrentTenantId();
        for (String id : ids) {
            // 检查记录是否存在（限定租户）
            ClnConditionDefinitionDO existing = lambdaQuery()
                    .eq(ClnConditionDefinitionDO::getId, id)
                    .eq(ClnConditionDefinitionDO::getTenantId, tenantId)
                    .one();
            if (existing == null) {
                throw new RuntimeException("诊断记录不存在，ID: " + id);
            }
            // 执行逻辑删除
            baseMapper.deleteById(id);
        }
    }

    @Override
    public Page<ClnConditionDefinitionDO> getSelectList(DiagnosisDefinitionQuery query) {
        Page<ClnConditionDefinitionDO> page = new Page<>(query.getPageIndex(), query.getPageSize());
        LambdaQueryWrapper<ClnConditionDefinitionDO> wrapper = new LambdaQueryWrapper<>();
        wrapper.eq(ClnConditionDefinitionDO::getTenantId, userHolder.getCurrentTenantId());
        
        if (StrUtil.isNotBlank(query.getName())) {
            wrapper.like(ClnConditionDefinitionDO::getName, query.getName());
        }
        if (StrUtil.isNotBlank(query.getPyStr())) {
            wrapper.like(ClnConditionDefinitionDO::getPyStr, query.getPyStr());
        }
        if (StrUtil.isNotBlank(query.getWbStr())) {
            wrapper.like(ClnConditionDefinitionDO::getWbStr, query.getWbStr());
        }
        
        return baseMapper.selectPage(page, wrapper);
    }

    @Override
    public PageDTO<DiagnosisOptionVO> queryDiagnosisSelectList(DiagnosisSelectPageQuery query) {
        // 诊断目录接口由其他成员负责，当前仅返回合法的空分页占位结果。
        Page<DiagnosisOptionVO> result = new Page<>(query.getPageIndex(), query.getPageSize(), 0L);
        result.setRecords(List.of());
        return PageDTO.create(result);
    }

    @Override
    public PageDTO<DiagnosisListVO> queryDiagnosisList(DiagnosisPageQuery query) {
        // 1. 处理分页参数
        long pageIndex = query.getPageIndex();
        long pageSize = query.getPageSize();
        Page<ClnConditionDefinitionDO> page = new Page<>(pageIndex, pageSize);

        // 2. 按关键字、诊断目录和当前租户构造组合查询条件
        LambdaQueryWrapper<ClnConditionDefinitionDO> wrapper = buildQueryWrapper(
                query.getKeyword(), query.getSourceEnum());
        wrapper.orderByAsc(ClnConditionDefinitionDO::getConditionCode)
                .orderByAsc(ClnConditionDefinitionDO::getId);

        // 3. 按诊断编码和主键稳定排序后查询诊断定义表
        Page<ClnConditionDefinitionDO> result = baseMapper.selectPage(page, wrapper);

        // 4. 将数据库实体转换为列表 VO，避免向前端暴露系统字段
        return PageDTO.create(result, msDiagnosisMapper::entityToListVo);
    }

    @Override
    public DiagnosisDetailVO queryDiagnosisDetail(String id) {
        // 1. 校验诊断 ID，防止绕过 Controller 时发起无效查询
        if (StrUtil.isBlank(id)) {
            throw new IllegalArgumentException("诊断ID不能为空");
        }
        ClnConditionDefinitionDO entity = lambdaQuery()
                .eq(ClnConditionDefinitionDO::getId, id)
                .eq(ClnConditionDefinitionDO::getTenantId, userHolder.getCurrentTenantId())
                .one();
        if (entity == null) {
            throw new RuntimeException("诊断记录不存在");
        }

        // 2. 按诊断 ID 和当前租户查询，避免跨租户读取
        return msDiagnosisMapper.entityToDiagnosisDetailVo(entity);
    }

    private LambdaQueryWrapper<ClnConditionDefinitionDO> buildQueryWrapper(
            String keyword, String sourceEnum) {
        // 关键字条件使用括号分组，确保 OR 只作用于名称、编码、拼音和五笔字段
        String normalizedKeyword = StrUtil.trim(keyword);
        String normalizedSourceEnum = StrUtil.trim(sourceEnum);
        LambdaQueryWrapper<ClnConditionDefinitionDO> wrapper = new LambdaQueryWrapper<>();
        wrapper.eq(ClnConditionDefinitionDO::getTenantId, userHolder.getCurrentTenantId());
        if (StrUtil.isNotBlank(normalizedKeyword)) {
            wrapper.and(item -> item
                    .like(ClnConditionDefinitionDO::getName, normalizedKeyword)
                    .or()
                    .like(ClnConditionDefinitionDO::getConditionCode, normalizedKeyword)
                    .or()
                    .like(ClnConditionDefinitionDO::getPyStr, normalizedKeyword)
                    .or()
                    .like(ClnConditionDefinitionDO::getWbStr, normalizedKeyword));
        }
        if (StrUtil.isNotBlank(normalizedSourceEnum)) {
            wrapper.eq(ClnConditionDefinitionDO::getSourceEnum, normalizedSourceEnum);
        }
        return wrapper;
    }

}
