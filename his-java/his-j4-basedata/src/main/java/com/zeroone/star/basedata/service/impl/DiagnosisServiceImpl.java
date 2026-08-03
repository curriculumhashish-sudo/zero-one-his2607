package com.zeroone.star.basedata.service.impl;

import cn.hutool.core.bean.BeanUtil;
import cn.hutool.core.util.StrUtil;
import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.core.conditions.update.LambdaUpdateWrapper;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.basedata.constant.DiagnosisConstants;
import com.zeroone.star.basedata.entity.diagnosis.ClnConditionDefinitionDO;
import com.zeroone.star.basedata.entity.diagnosis.TerConceptDO;
import com.zeroone.star.basedata.mapper.DiagnosisCatalogMapper;
import com.zeroone.star.basedata.mapper.DiagnosisMapper;
import com.zeroone.star.basedata.service.IDiagnosisService;

import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j4.diagnosis.DiagnosisDefinitionDTO;
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
    private DiagnosisCatalogMapper diagnosisCatalogMapper;

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
        // 1. 获取当前租户ID并校验病情编码唯一
        String tenantId = userHolder.getCurrentTenantId();
        checkConditionCodeUnique(dto.getConditionCode(), null, tenantId);
        
        // 2. DTO转实体
        ClnConditionDefinitionDO entity = new ClnConditionDefinitionDO();
        BeanUtil.copyProperties(dto, entity);
        
        // 设置创建时间
        entity.setCreatedAt(LocalDateTime.now());
        entity.setUpdatedAt(LocalDateTime.now());
        
        // 设置租户ID
        entity.setTenantId(tenantId);
        
        // 新增默认状态为"已启用"
        entity.setStatusEnum(DiagnosisConstants.STATUS_ACTIVE);
        
        // 3. 保存到数据库
        baseMapper.insert(entity);
        
        // 4. 返回新增记录ID
        return entity.getId();
    }
    
    /**
     * 校验同租户内病情编码是否唯一
     * @param conditionCode 病情编码
     * @param excludeId     需要排除的主键ID（新增传null，修改传当前ID）
     * @param tenantId      当前租户ID
     */
    private void checkConditionCodeUnique(String conditionCode, String excludeId, String tenantId) {
        Long count = lambdaQuery()
                .eq(ClnConditionDefinitionDO::getTenantId, tenantId)
                .eq(ClnConditionDefinitionDO::getConditionCode, conditionCode)
                .ne(excludeId != null, ClnConditionDefinitionDO::getId, excludeId)
                .count();
        if (count != null && count > 0) {
            throw new RuntimeException("病情编码 [" + conditionCode + "] 已存在");
        }
    }

    @Override
    @Transactional
    public void modifyDefinition(DiagnosisUpdateDTO dto) {
        // 1. 获取当前租户ID并检查记录是否存在
        String tenantId = userHolder.getCurrentTenantId();
        ClnConditionDefinitionDO existing = baseMapper.selectById(dto.getId());
        if (existing == null) {
            throw new RuntimeException("诊断记录不存在");
        }
        // 2. 校验同租户内病情编码唯一（排除自身）
        checkConditionCodeUnique(dto.getConditionCode(), dto.getId(), tenantId);
        // 3. DTO转实体
        ClnConditionDefinitionDO entity = new ClnConditionDefinitionDO();
        BeanUtil.copyProperties(dto, entity);
        // 4. 保留系统字段
        entity.setCreatedAt(existing.getCreatedAt());
        entity.setCreatedBy(existing.getCreatedBy());
        entity.setIsDeleted(existing.getIsDeleted());
        entity.setDeletedAt(existing.getDeletedAt());
        entity.setDeletedBy(existing.getDeletedBy());
        entity.setVersion(existing.getVersion());
        entity.setTenantId(existing.getTenantId());
        entity.setUpdatedAt(LocalDateTime.now());
        // 5. 更新数据库
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
        if (!DiagnosisConstants.isValidStatus(dto.getStatus())) {
            throw new IllegalArgumentException("状态值无效，必须为 DRAFT / ACTIVE / INACTIVE / DISCARDED");
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
        Page<TerConceptDO> page = new Page<>(query.getPageIndex(), query.getPageSize());
        LambdaQueryWrapper<TerConceptDO> wrapper = new LambdaQueryWrapper<>();
        wrapper.eq(TerConceptDO::getTenantId, userHolder.getCurrentTenantId())
                .eq(TerConceptDO::getCodeSystemId, "279125561775755266")
                .eq(TerConceptDO::getIsActive, true);

        if (StrUtil.isNotBlank(query.getKeyword())) {
            wrapper.and(item -> item
                    .like(TerConceptDO::getDisplay, query.getKeyword())
                    .or()
                    .like(TerConceptDO::getCode, query.getKeyword()));
        }
        if (StrUtil.isNotBlank(query.getSourceEnum())) {
            wrapper.eq(TerConceptDO::getCode, query.getSourceEnum());
        }
        wrapper.orderByAsc(TerConceptDO::getSortOrder)
                .orderByAsc(TerConceptDO::getId);

        Page<TerConceptDO> result = diagnosisCatalogMapper.selectPage(page, wrapper);
        return PageDTO.create(result, msDiagnosisMapper::conceptToOptionVo);
    }

    @Override
    public PageDTO<DiagnosisListVO> queryDiagnosisList(DiagnosisPageQuery query) {
        Page<ClnConditionDefinitionDO> page = new Page<>(query.getPageIndex(), query.getPageSize());
        LambdaQueryWrapper<ClnConditionDefinitionDO> wrapper = buildQueryWrapper(
                query.getKeyword(), query.getSourceEnum());
        wrapper.orderByAsc(ClnConditionDefinitionDO::getConditionCode);

        Page<ClnConditionDefinitionDO> result = baseMapper.selectPage(page, wrapper);
        return PageDTO.create(result, msDiagnosisMapper::entityToListVo);
    }

    @Override
    public DiagnosisDetailVO queryDiagnosisDetail(String id) {
        ClnConditionDefinitionDO entity = baseMapper.selectById(id);
        return entity == null ? null : msDiagnosisMapper.entityToDiagnosisDetailVo(entity);
    }

    private LambdaQueryWrapper<ClnConditionDefinitionDO> buildQueryWrapper(
            String keyword, String sourceEnum) {
        LambdaQueryWrapper<ClnConditionDefinitionDO> wrapper = new LambdaQueryWrapper<>();
        wrapper.eq(ClnConditionDefinitionDO::getTenantId, userHolder.getCurrentTenantId());
        if (StrUtil.isNotBlank(keyword)) {
            wrapper.and(item -> item
                    .like(ClnConditionDefinitionDO::getName, keyword)
                    .or()
                    .like(ClnConditionDefinitionDO::getConditionCode, keyword)
                    .or()
                    .like(ClnConditionDefinitionDO::getPyStr, keyword)
                    .or()
                    .like(ClnConditionDefinitionDO::getWbStr, keyword));
        }
        if (StrUtil.isNotBlank(sourceEnum)) {
            wrapper.eq(ClnConditionDefinitionDO::getSourceEnum, sourceEnum);
        }
        return wrapper;
    }

}
