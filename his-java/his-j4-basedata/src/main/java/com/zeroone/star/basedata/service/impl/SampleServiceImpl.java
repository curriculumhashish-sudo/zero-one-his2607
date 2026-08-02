package com.zeroone.star.basedata.service.impl;

import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.basedata.constant.SampleConstants;
import com.zeroone.star.basedata.entity.sample.AdmSpecimenDefinitionDO;
import com.zeroone.star.basedata.mapper.SampleMapper;
import com.zeroone.star.basedata.service.ISampleService;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j4.sample.SpecimenDefinitionAddDTO;
import com.zeroone.star.project.dto.j4.sample.SpecimenDefinitionDeleteDTO;
import com.zeroone.star.project.dto.j4.sample.SpecimenDefinitionStatusDTO;
import com.zeroone.star.project.dto.j4.sample.SpecimenDefinitionUpdateDTO;
import com.zeroone.star.project.query.j4.sample.SpecimenDefinitionQuery;
import com.zeroone.star.project.vo.j4.sample.SpecimenDefinitionDetailVO;
import com.zeroone.star.project.vo.j4.sample.SpecimenDefinitionOperateVO;
import com.zeroone.star.project.vo.j4.sample.SpecimenOptionVO;
import jakarta.annotation.Resource;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.time.LocalDateTime;
import java.util.List;
import java.util.Objects;

/**
 * <p>
 * 描述：样本管理服务实现
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@Service
public class SampleServiceImpl extends ServiceImpl<SampleMapper, AdmSpecimenDefinitionDO> implements ISampleService {

    @Resource
    private MsSampleMapper msSampleMapper;

    // UserHolder用于获取当前租户ID
    @Resource
    private UserHolder userHolder;

    @Override
    public List<SpecimenOptionVO> listSampleNames() {
        // 1. 获取当前租户ID
//        String tenantId = userHolder.getCurrentTenantId();
        String tenantId = "1";
        // 2. 查询当前租户下所有未删除样本的名称下拉选项
        return baseMapper.selectSampleOptions(tenantId);
    }

    @Override
    public PageDTO<SpecimenDefinitionDetailVO> querySamplePage(SpecimenDefinitionQuery query) {
        // 1. 获取当前租户ID
//        String tenantId = userHolder.getCurrentTenantId();
        String tenantId = "1";
        // 2. 构建分页对象并按租户查询
        Page<SpecimenDefinitionDetailVO> page = new Page<>(query.getPageIndex(), query.getPageSize());
        Page<SpecimenDefinitionDetailVO> result = baseMapper.selectSamplePage(page, query, tenantId);
        // 3. 填充枚举中文名
        result.getRecords().forEach(vo -> {
            vo.setTypeName(SampleConstants.getTypeName(vo.getTypeEnum()));
            vo.setStatusName(SampleConstants.getStatusName(vo.getStatusEnum()));
        });
        return PageDTO.create(result);
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public String addSample(SpecimenDefinitionAddDTO dto) {
        // 1. 获取当前租户ID
//        String tenantId = userHolder.getCurrentTenantId();
        String tenantId = "1";
        // 2. 校验同租户内业务编码唯一
        checkBusNoUnique(dto.getBusNo(), null, tenantId);
        // 3. DTO转实体并绑定租户，状态调用 PublicationStatus 枚举默认置为启用
        AdmSpecimenDefinitionDO entity = msSampleMapper.saveDtoToEntity(dto);
        entity.setTenantId(tenantId);
        entity.setStatusEnum(SampleConstants.STATUS_ACTIVE);
        // 4. 保存到数据库
        baseMapper.insert(entity);
        return "新增样本定义成功";
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public SpecimenDefinitionOperateVO updateSample(SpecimenDefinitionUpdateDTO dto) {
        // 1. 获取当前租户ID并校验记录归属
//        String tenantId = userHolder.getCurrentTenantId();
        String tenantId = "1";
        AdmSpecimenDefinitionDO existing = baseMapper.selectById(dto.getId());
        if (existing == null || !tenantId.equals(existing.getTenantId())) {
            throw new RuntimeException("样本记录不存在");
        }
        // 2. 校验同租户内业务编码唯一（排除自身）
        checkBusNoUnique(dto.getBusNo(), dto.getId(), tenantId);
        // 3. DTO转实体
        AdmSpecimenDefinitionDO entity = msSampleMapper.updateDtoToEntity(dto);
        // 4. 保留系统字段、租户标识与状态（状态不参与修改）
        entity.setTenantId(existing.getTenantId());
        entity.setStatusEnum(existing.getStatusEnum());
        entity.setCreatedAt(existing.getCreatedAt());
        entity.setCreatedBy(existing.getCreatedBy());
        entity.setIsDeleted(existing.getIsDeleted());
        entity.setDeletedAt(existing.getDeletedAt());
        entity.setDeletedBy(existing.getDeletedBy());
        entity.setVersion(existing.getVersion());
        // 5. 更新数据库（乐观锁：version 不匹配时影响0行）
        int updated = baseMapper.updateById(entity);
        if (updated == 0) {
            // 区分并发冲突与"值未变化"：仅当 version 已变化时才判定为并发冲突
            AdmSpecimenDefinitionDO latest = baseMapper.selectById(dto.getId());
            if (latest == null || !Objects.equals(latest.getVersion(), existing.getVersion())) {
                throw new RuntimeException("数据已被他人修改，请刷新后重试");
            }
        }
        // 6. 查询最新数据并返回操作结果
        return buildOperateVO(baseMapper.selectById(dto.getId()));
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public SpecimenDefinitionOperateVO deleteSample(SpecimenDefinitionDeleteDTO dto) {
        // 1. 获取当前租户ID
//        String tenantId = userHolder.getCurrentTenantId();
        String tenantId = "1";
        // 2. 带租户条件的逻辑删除（@TableLogic 自动转为 update is_deleted = 1），并填充删除审计信息
        boolean removed = lambdaUpdate()
                .eq(AdmSpecimenDefinitionDO::getId, dto.getId())
                .eq(AdmSpecimenDefinitionDO::getTenantId, tenantId)
                .set(AdmSpecimenDefinitionDO::getDeletedAt, LocalDateTime.now())
                .set(AdmSpecimenDefinitionDO::getDeletedBy, currentUsername())
                .remove();
        if (!removed) {
            throw new RuntimeException("样本记录不存在");
        }
        // 3. 返回操作结果（删除后记录已软删，version 返回入参值）
        SpecimenDefinitionOperateVO vo = new SpecimenDefinitionOperateVO();
        vo.setId(dto.getId());
        vo.setVersion(dto.getVersion());
        return vo;
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public SpecimenDefinitionOperateVO updateSampleStatus(SpecimenDefinitionStatusDTO dto) {
        // 1. 获取当前租户ID并校验记录归属
//        String tenantId = userHolder.getCurrentTenantId();
        String tenantId = "1";
        AdmSpecimenDefinitionDO existing = baseMapper.selectById(dto.getId());
        if (existing == null || !tenantId.equals(existing.getTenantId())) {
            throw new RuntimeException("样本记录不存在");
        }
        // 2. 状态未变化时直接返回成功（幂等）
        if (existing.getStatusEnum() != null && existing.getStatusEnum().equals(dto.getStatusEnum())) {
            return buildOperateVO(existing);
        }
        // 3. 更新状态（限定当前租户）
        boolean updated = lambdaUpdate()
                .eq(AdmSpecimenDefinitionDO::getId, dto.getId())
                .eq(AdmSpecimenDefinitionDO::getTenantId, tenantId)
                .set(AdmSpecimenDefinitionDO::getStatusEnum, dto.getStatusEnum())
                .update();
        if (!updated) {
            throw new RuntimeException("样本记录不存在");
        }
        // 4. 查询最新数据并返回操作结果
        return buildOperateVO(baseMapper.selectById(dto.getId()));
    }

    /**
     * 校验同租户内样本业务编码是否唯一
     *
     * @param busNo     业务编码
     * @param excludeId 需要排除的主键ID（新增传null，修改传当前ID）
     * @param tenantId  当前租户ID
     */
    private void checkBusNoUnique(String busNo, String excludeId, String tenantId) {
        Long count = lambdaQuery()
                .eq(AdmSpecimenDefinitionDO::getTenantId, tenantId)
                .eq(AdmSpecimenDefinitionDO::getBusNo, busNo)
                .ne(excludeId != null, AdmSpecimenDefinitionDO::getId, excludeId)
                .count();
        if (count != null && count > 0) {
            throw new RuntimeException("业务编码 [" + busNo + "] 已存在");
        }
    }

    /**
     * 构建操作结果 VO
     *
     * @param entity 最新样本实体
     * @return 操作结果
     */
    private SpecimenDefinitionOperateVO buildOperateVO(AdmSpecimenDefinitionDO entity) {
        SpecimenDefinitionOperateVO vo = new SpecimenDefinitionOperateVO();
        if (entity != null) {
            vo.setId(entity.getId());
            vo.setStatusEnum(entity.getStatusEnum());
            vo.setVersion(entity.getVersion());
        }
        return vo;
    }

    /**
     * 获取当前登录用户名，获取失败时回退为 system
     *
     * @return 用户名
     */
    private String currentUsername() {
        try {
            return userHolder.getCurrentUser() != null
                    ? userHolder.getCurrentUser().getUsername()
                    : "system";
        } catch (Exception e) {
            return "system";
        }
    }
}
