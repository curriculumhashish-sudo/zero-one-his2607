package com.zeroone.star.basedata.service.impl;

import cn.hutool.core.util.StrUtil;
import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.basedata.entity.common.AdmChargeItemDefinitionDO;
import com.zeroone.star.basedata.entity.treatment.WkfActivityDefinitionChildDO;
import com.zeroone.star.basedata.entity.treatment.WkfActivityDefinitionDO;
import com.zeroone.star.basedata.mapper.AdmChargeItemDefinitionMapper;
import com.zeroone.star.basedata.mapper.TreatmentMapper;
import com.zeroone.star.basedata.mapper.WkfActivityDefinitionChildMapper;
import com.zeroone.star.basedata.service.ITreatmentService;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j4.treatment.TreatmentAddDTO;
import com.zeroone.star.project.dto.j4.treatment.TreatmentEditDTO;
import com.zeroone.star.project.dto.j4.treatment.TreatmentStatusDTO;
import com.zeroone.star.project.query.j4.treatment.TreatmentQuery;
import com.zeroone.star.project.vo.j4.treatment.TreatmentDetailVO;
import com.zeroone.star.project.vo.j4.treatment.TreatmentListVO;
import com.zeroone.star.project.vo.j4.treatment.TreatmentSelectVO;
import com.zeroone.star.project.vo.j4.treatment.WkfActivityDefinitionChildVO;
import jakarta.annotation.Resource;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.math.BigDecimal;
import java.time.LocalDateTime;
import java.util.Collections;
import java.util.List;

/**
 * <p>
 * 描述：诊疗管理服务实现
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@Service
public class TreatmentServiceImpl extends ServiceImpl<TreatmentMapper, WkfActivityDefinitionDO> implements ITreatmentService {

    private static final String INSTANCE_TABLE = "wkf_activity_definition";

    private static final String STATUS_ACTIVE = "ACTIVE";
    private static final String STATUS_INACTIVE = "INACTIVE";

    private static final String PRICE_TYPE_PURCHASE = "PURCHASE";
    private static final String PRICE_TYPE_SALE = "SALE";
    private static final String PRICE_TYPE_LIMIT = "LIMIT";

    private static final long DEFAULT_PAGE_INDEX = 1L;
    private static final long DEFAULT_PAGE_SIZE = 10L;

    @Resource
    private MsTreatmentMapper msTreatmentMapper;

    @Resource
    private AdmChargeItemDefinitionMapper chargeItemMapper;

    @Resource
    private WkfActivityDefinitionChildMapper childMapper;

    @Resource
    private UserHolder userHolder;

    /**
     * 获取当前登录用户名，获取失败时回退为 system
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

    @Override
    public PageDTO<TreatmentSelectVO> listTreatmentOptions(TreatmentQuery query) {
        long pageIndex = (query.getPageIndex() <= 0) ? DEFAULT_PAGE_INDEX : query.getPageIndex();
        long pageSize = (query.getPageSize() <= 0) ? DEFAULT_PAGE_SIZE : query.getPageSize();

        String tenantId = userHolder.getCurrentTenantId();
        Page<WkfActivityDefinitionDO> page = new Page<>(pageIndex, pageSize);
        LambdaQueryWrapper<WkfActivityDefinitionDO> wrapper = new LambdaQueryWrapper<>();
        wrapper.select(WkfActivityDefinitionDO::getId, WkfActivityDefinitionDO::getBusNo,
                WkfActivityDefinitionDO::getName, WkfActivityDefinitionDO::getCategoryCode,
                WkfActivityDefinitionDO::getTypeEnum);
        wrapper.eq(WkfActivityDefinitionDO::getTenantId, tenantId);
        wrapper.eq(WkfActivityDefinitionDO::getStatusEnum, STATUS_ACTIVE);
        if (StrUtil.isNotBlank(query.getKeyword())) {
            wrapper.and(w -> w
                    .like(WkfActivityDefinitionDO::getName, query.getKeyword())
                    .or()
                    .like(WkfActivityDefinitionDO::getBusNo, query.getKeyword())
                    .or()
                    .like(WkfActivityDefinitionDO::getPyStr, query.getKeyword())
                    .or()
                    .like(WkfActivityDefinitionDO::getWbStr, query.getKeyword())
            );
        }
        if (StrUtil.isNotBlank(query.getCategoryCode())) {
            wrapper.eq(WkfActivityDefinitionDO::getCategoryCode, query.getCategoryCode());
        }
        wrapper.orderByDesc(WkfActivityDefinitionDO::getCreatedAt);
        Page<WkfActivityDefinitionDO> result = baseMapper.selectPage(page, wrapper);
        return PageDTO.create(result, msTreatmentMapper::toSelectVO);
    }

    @Override
    public PageDTO<TreatmentListVO> queryTreatmentList(TreatmentQuery query) {
        long pageIndex = query.getPageIndex() <= 0 ? DEFAULT_PAGE_INDEX : query.getPageIndex();
        long pageSize = query.getPageSize() <= 0 ? DEFAULT_PAGE_SIZE : query.getPageSize();

        String tenantId = userHolder.getCurrentTenantId();
        Page<TreatmentListVO> page = new Page<>(pageIndex, pageSize);
        Page<TreatmentListVO> result = baseMapper.selectTreatmentListPage(page, query, tenantId);
        return PageDTO.create(result);
    }

    @Override
    public TreatmentDetailVO getTreatmentById(String id) {
        String tenantId = userHolder.getCurrentTenantId();

        LambdaQueryWrapper<WkfActivityDefinitionDO> wrapper = new LambdaQueryWrapper<>();
        wrapper.select(WkfActivityDefinitionDO::getId, WkfActivityDefinitionDO::getBusNo,
                        WkfActivityDefinitionDO::getName, WkfActivityDefinitionDO::getCategoryCode,
                        WkfActivityDefinitionDO::getTypeEnum, WkfActivityDefinitionDO::getPackageUnitCode,
                        WkfActivityDefinitionDO::getPyStr, WkfActivityDefinitionDO::getWbStr,
                        WkfActivityDefinitionDO::getDescriptionText, WkfActivityDefinitionDO::getYbFlag,
                        WkfActivityDefinitionDO::getYbNo, WkfActivityDefinitionDO::getYbMatchFlag,
                        WkfActivityDefinitionDO::getChrgitmLv, WkfActivityDefinitionDO::getStatusEnum)
                .eq(WkfActivityDefinitionDO::getId, id)
                .eq(WkfActivityDefinitionDO::getTenantId, tenantId);
        WkfActivityDefinitionDO entity = this.getOne(wrapper);
        if (entity == null) {
            throw new RuntimeException("诊疗项目不存在");
        }

        TreatmentDetailVO detailVO = msTreatmentMapper.toDetailVO(entity);

        List<AdmChargeItemDefinitionDO> priceLines = loadPriceLines(id, tenantId);
        enrichPriceFields(detailVO, priceLines);

        List<WkfActivityDefinitionChildDO> children = loadChildren(id, tenantId);
        List<WkfActivityDefinitionChildVO> childVOs = msTreatmentMapper.toChildVOList(children);
        detailVO.setChildren(childVOs);

        return detailVO;
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public void addTreatment(TreatmentAddDTO dto) {
        String tenantId = userHolder.getCurrentTenantId();

        WkfActivityDefinitionDO entity = msTreatmentMapper.addDtoToEntity(dto);
        entity.setTenantId(tenantId);
        // 所属机构/位置由登录上下文决定，不信任前端传入
        entity.setOrgId(userHolder.getCurrentDeptId());
        entity.setLocationId(userHolder.getCurrentLocationId());

        // 默认状态ACTIVE
        if (StrUtil.isBlank(entity.getStatusEnum())) {
            entity.setStatusEnum(STATUS_ACTIVE);
        }

        // 自动判断childrenFlag
        List<WkfActivityDefinitionChildVO> children = dto.getChildren();
        if (entity.getChildrenFlag() == null) {
            entity.setChildrenFlag((children != null && !children.isEmpty()) ? 1 : 0);
        }

        this.save(entity);
        String newId = entity.getId();

        // 保存价格记录到收费表
        savePriceLines(newId, tenantId, dto.getTypeCode(), dto.getYbType(),
                dto.getPurchasePrice(), dto.getRetailPrice(), dto.getMaximumRetailPrice());

        // 保存子项记录
        if (children != null && !children.isEmpty()) {
            saveChildren(newId, tenantId, children);
        }
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public TreatmentDetailVO modifyTreatment(TreatmentEditDTO dto) {
        String tenantId = userHolder.getCurrentTenantId();

        WkfActivityDefinitionDO existing = this.getById(dto.getId());
        if (existing == null || !tenantId.equals(existing.getTenantId())) {
            throw new RuntimeException("诊疗项目不存在");
        }

        WkfActivityDefinitionDO entity = msTreatmentMapper.editDtoToEntity(dto);
        entity.setTenantId(existing.getTenantId());
        entity.setCreatedAt(existing.getCreatedAt());
        entity.setCreatedBy(existing.getCreatedBy());
        entity.setIsDeleted(existing.getIsDeleted());
        entity.setDeletedAt(existing.getDeletedAt());
        entity.setDeletedBy(existing.getDeletedBy());
        entity.setVersion(existing.getVersion());
        this.updateById(entity);

        // 处理价格：先逻辑删除旧价格，再插入新价格
        deletePriceLines(dto.getId(), tenantId);
        savePriceLines(dto.getId(), tenantId, dto.getTypeCode(), dto.getYbType(),
                dto.getPurchasePrice(), dto.getRetailPrice(), dto.getMaximumRetailPrice());

        // 处理子项：先逻辑删除旧子项，再插入新子项
        deleteChildren(dto.getId(), tenantId);
        List<WkfActivityDefinitionChildVO> children = dto.getChildren();
        if (children != null && !children.isEmpty()) {
            saveChildren(dto.getId(), tenantId, children);
        }
        // 自动更新childrenFlag
        if (dto.getChildrenFlag() == null) {
            entity.setChildrenFlag((children != null && !children.isEmpty()) ? 1 : 0);
            this.updateById(entity);
        }

        // 重新查询返回最新详情
        return getTreatmentById(dto.getId());
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public TreatmentDetailVO modifyStatus(TreatmentStatusDTO dto) {
        String tenantId = userHolder.getCurrentTenantId();

        WkfActivityDefinitionDO existing = this.getById(dto.getId());
        if (existing == null || !tenantId.equals(existing.getTenantId())) {
            throw new RuntimeException("诊疗项目不存在");
        }

        // 状态未变化直接返回
        if (existing.getStatusEnum() != null && existing.getStatusEnum().equals(dto.getStatusEnum())) {
            return getTreatmentById(dto.getId());
        }

        lambdaUpdate()
                .eq(WkfActivityDefinitionDO::getId, dto.getId())
                .eq(WkfActivityDefinitionDO::getTenantId, tenantId)
                .set(WkfActivityDefinitionDO::getStatusEnum, dto.getStatusEnum())
                .update();

        return getTreatmentById(dto.getId());
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public void removeTreatment(String id) {
        String tenantId = userHolder.getCurrentTenantId();

        boolean removed = lambdaUpdate()
                .eq(WkfActivityDefinitionDO::getId, id)
                .eq(WkfActivityDefinitionDO::getTenantId, tenantId)
                .set(WkfActivityDefinitionDO::getDeletedAt, LocalDateTime.now())
                .set(WkfActivityDefinitionDO::getDeletedBy, currentUsername())
                .remove();
        if (!removed) {
            throw new RuntimeException("诊疗项目不存在");
        }

        // 同时逻辑删除关联的价格记录
        deletePriceLines(id, tenantId);
        // 同时逻辑删除关联的子项记录
        deleteChildren(id, tenantId);
    }

    // ==================== 私有辅助方法 ====================

    private List<AdmChargeItemDefinitionDO> loadPriceLines(String instanceId, String tenantId) {
        LambdaQueryWrapper<AdmChargeItemDefinitionDO> wrapper = new LambdaQueryWrapper<>();
        wrapper.select(AdmChargeItemDefinitionDO::getTypeCode, AdmChargeItemDefinitionDO::getYbType,
                AdmChargeItemDefinitionDO::getPriceTypeCode, AdmChargeItemDefinitionDO::getPriceValue);
        wrapper.eq(AdmChargeItemDefinitionDO::getInstanceTable, INSTANCE_TABLE)
                .eq(AdmChargeItemDefinitionDO::getInstanceId, instanceId)
                .eq(AdmChargeItemDefinitionDO::getIsDeleted, false)
                .eq(AdmChargeItemDefinitionDO::getTenantId, tenantId);
        return chargeItemMapper.selectList(wrapper);
    }

    private void enrichPriceFields(TreatmentDetailVO dto, List<AdmChargeItemDefinitionDO> rows) {
        if (dto == null || rows == null || rows.isEmpty()) {
            return;
        }
        for (AdmChargeItemDefinitionDO row : rows) {
            if (row == null) continue;
            if (dto.getTypeCode() == null && row.getTypeCode() != null) {
                dto.setTypeCode(row.getTypeCode());
            }
            if (dto.getYbType() == null && row.getYbType() != null) {
                dto.setYbType(row.getYbType());
            }
            if (PRICE_TYPE_PURCHASE.equals(row.getPriceTypeCode())) {
                dto.setPurchasePrice(row.getPriceValue());
            } else if (PRICE_TYPE_LIMIT.equals(row.getPriceTypeCode())) {
                dto.setMaximumRetailPrice(row.getPriceValue());
            } else if (PRICE_TYPE_SALE.equals(row.getPriceTypeCode())) {
                dto.setRetailPrice(row.getPriceValue());
            }
        }
    }

    /**
     * 保存价格记录到收费表
     */
    private void savePriceLines(String instanceId, String tenantId, String typeCode, String ybType,
                                BigDecimal purchasePrice, BigDecimal retailPrice, BigDecimal maximumRetailPrice) {
        String username = currentUsername();
        LocalDateTime now = LocalDateTime.now();

        // 成本价
        if (purchasePrice != null) {
            AdmChargeItemDefinitionDO purchase = new AdmChargeItemDefinitionDO();
            purchase.setInstanceTable(INSTANCE_TABLE);
            purchase.setInstanceId(instanceId);
            purchase.setTypeCode(typeCode);
            purchase.setYbType(ybType);
            purchase.setPriceTypeCode(PRICE_TYPE_PURCHASE);
            purchase.setPriceValue(purchasePrice);
            purchase.setStatusEnum(STATUS_ACTIVE);
            purchase.setTenantId(tenantId);
            chargeItemMapper.insert(purchase);
        }

        // 零售价
        if (retailPrice != null) {
            AdmChargeItemDefinitionDO sale = new AdmChargeItemDefinitionDO();
            sale.setInstanceTable(INSTANCE_TABLE);
            sale.setInstanceId(instanceId);
            sale.setTypeCode(typeCode);
            sale.setYbType(ybType);
            sale.setPriceTypeCode(PRICE_TYPE_SALE);
            sale.setPriceValue(retailPrice);
            sale.setStatusEnum(STATUS_ACTIVE);
            sale.setTenantId(tenantId);
            chargeItemMapper.insert(sale);
        }

        // 最高零售价
        if (maximumRetailPrice != null) {
            AdmChargeItemDefinitionDO limit = new AdmChargeItemDefinitionDO();
            limit.setInstanceTable(INSTANCE_TABLE);
            limit.setInstanceId(instanceId);
            limit.setTypeCode(typeCode);
            limit.setYbType(ybType);
            limit.setPriceTypeCode(PRICE_TYPE_LIMIT);
            limit.setPriceValue(maximumRetailPrice);
            limit.setStatusEnum(STATUS_ACTIVE);
            limit.setTenantId(tenantId);
            chargeItemMapper.insert(limit);
        }
    }

    /**
     * 逻辑删除关联的价格记录
     */
    private void deletePriceLines(String instanceId, String tenantId) {
        LambdaQueryWrapper<AdmChargeItemDefinitionDO> wrapper = new LambdaQueryWrapper<>();
        wrapper.eq(AdmChargeItemDefinitionDO::getInstanceTable, INSTANCE_TABLE)
                .eq(AdmChargeItemDefinitionDO::getInstanceId, instanceId)
                .eq(AdmChargeItemDefinitionDO::getTenantId, tenantId)
                .eq(AdmChargeItemDefinitionDO::getIsDeleted, false);
        List<AdmChargeItemDefinitionDO> oldLines = chargeItemMapper.selectList(wrapper);
        if (oldLines != null && !oldLines.isEmpty()) {
            for (AdmChargeItemDefinitionDO line : oldLines) {
                chargeItemMapper.deleteById(line.getId());
            }
        }
    }

    private List<WkfActivityDefinitionChildDO> loadChildren(String activityId, String tenantId) {
        LambdaQueryWrapper<WkfActivityDefinitionChildDO> wrapper = new LambdaQueryWrapper<>();
        wrapper.select(WkfActivityDefinitionChildDO::getId, WkfActivityDefinitionChildDO::getActivityId,
                WkfActivityDefinitionChildDO::getChildrenId, WkfActivityDefinitionChildDO::getChildrenNum,
                WkfActivityDefinitionChildDO::getChildrenName);
        wrapper.eq(WkfActivityDefinitionChildDO::getActivityId, activityId)
                .eq(WkfActivityDefinitionChildDO::getIsDeleted, 0)
                .eq(WkfActivityDefinitionChildDO::getTenantId, tenantId);
        List<WkfActivityDefinitionChildDO> children = childMapper.selectList(wrapper);
        return children != null ? children : Collections.emptyList();
    }

    /**
     * 保存子项记录
     */
    private void saveChildren(String activityId, String tenantId, List<WkfActivityDefinitionChildVO> childVOs) {
        List<WkfActivityDefinitionChildDO> childDOs = msTreatmentMapper.childVoListToEntityList(childVOs);
        for (WkfActivityDefinitionChildDO child : childDOs) {
            child.setActivityId(activityId);
            child.setTenantId(tenantId);
            childMapper.insert(child);
        }
    }

    /**
     * 逻辑删除关联的子项记录
     */
    private void deleteChildren(String activityId, String tenantId) {
        LambdaQueryWrapper<WkfActivityDefinitionChildDO> wrapper = new LambdaQueryWrapper<>();
        wrapper.eq(WkfActivityDefinitionChildDO::getActivityId, activityId)
                .eq(WkfActivityDefinitionChildDO::getTenantId, tenantId)
                .eq(WkfActivityDefinitionChildDO::getIsDeleted, 0);
        List<WkfActivityDefinitionChildDO> oldChildren = childMapper.selectList(wrapper);
        if (oldChildren != null && !oldChildren.isEmpty()) {
            for (WkfActivityDefinitionChildDO child : oldChildren) {
                childMapper.deleteById(child.getId());
            }
        }
    }
}
