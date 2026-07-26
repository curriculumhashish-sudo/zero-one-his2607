package com.zeroone.star.configuration.orderset.converter;

import com.baomidou.mybatisplus.core.toolkit.IdWorker;
import com.zeroone.star.configuration.orderset.domain.OrderSetDO;
import com.zeroone.star.configuration.orderset.domain.OrderSetLineDO;
import com.zeroone.star.configuration.orderset.enums.OrderSetStatusEnum;
import com.zeroone.star.project.dto.j5.configuration.orderset.OrderSetAddDTO;
import com.zeroone.star.project.dto.j5.configuration.orderset.OrderSetLineDTO;
import com.zeroone.star.project.dto.j5.configuration.orderset.OrderSetModifyDTO;
import com.zeroone.star.project.vo.j5.configuration.orderset.OrderSetOperationVO;

import java.time.LocalDateTime;

/**
 * 组套模板各层模型转换器。
 */
public final class OrderSetConverter {

    private OrderSetConverter() {
    }

    public static OrderSetDO fromAddDTO(OrderSetAddDTO dto, String tenantId, String userId) {
        LocalDateTime now = LocalDateTime.now();
        OrderSetDO entity = new OrderSetDO();
        entity.setId(IdWorker.getIdStr());
        entity.setTenantId(tenantId);
        entity.setCreatedAt(now);
        entity.setUpdatedAt(now);
        entity.setCreatedBy(userId);
        entity.setUpdatedBy(userId);
        entity.setIsDeleted(false);
        entity.setVersion(0);
        entity.setObjectId(dto.getObjectId());
        entity.setBindingEnum(dto.getBindingEnum());
        entity.setSceneEnum(dto.getSceneEnum());
        entity.setTemplateCode(dto.getTemplateCode());
        entity.setName(dto.getName());
        entity.setDescription(dto.getDescription());
        entity.setSortWeight(dto.getSortWeight());
        entity.setStatusEnum(OrderSetStatusEnum.DRAFT.name());
        entity.setRemark(dto.getRemark());
        entity.setTemplateKindEnum(dto.getTemplateKindEnum());
        entity.setDefaultChineseHerbsDoseQuantity(dto.getDefaultChineseHerbsDoseQuantity());
        entity.setDefaultSufferingFlag(dto.getDefaultSufferingFlag());
        entity.setTemplateSourceEnum("MAINTAINED");
        return entity;
    }

    public static void applyModifyDTO(OrderSetModifyDTO dto, OrderSetDO entity, String userId) {
        entity.setSceneEnum(dto.getSceneEnum());
        entity.setTemplateCode(dto.getTemplateCode());
        entity.setName(dto.getName());
        entity.setDescription(dto.getDescription());
        entity.setSortWeight(dto.getSortWeight());
        entity.setRemark(dto.getRemark());
        entity.setDefaultChineseHerbsDoseQuantity(dto.getDefaultChineseHerbsDoseQuantity());
        entity.setDefaultSufferingFlag(dto.getDefaultSufferingFlag());
        entity.setUpdatedBy(userId);
        entity.setUpdatedAt(LocalDateTime.now());
    }

    public static OrderSetLineDO newLine(OrderSetLineDTO dto, String templateId,
                                         String tenantId, String userId) {
        LocalDateTime now = LocalDateTime.now();
        OrderSetLineDO entity = new OrderSetLineDO();
        entity.setId(IdWorker.getIdStr());
        entity.setTenantId(tenantId);
        entity.setOrderTemplateId(templateId);
        entity.setCreatedAt(now);
        entity.setUpdatedAt(now);
        entity.setCreatedBy(userId);
        entity.setUpdatedBy(userId);
        entity.setIsDeleted(false);
        entity.setVersion(0);
        applyLineDTO(dto, entity, userId);
        return entity;
    }

    public static void applyLineDTO(OrderSetLineDTO dto, OrderSetLineDO entity, String userId) {
        entity.setLineKindEnum(dto.getLineKindEnum());
        entity.setMedicationDefinitionId(dto.getMedicationDefinitionId());
        entity.setActivityDefinitionId(dto.getActivityDefinitionId());
        entity.setDeviceDefinitionId(dto.getDeviceDefinitionId());
        entity.setGroupKey(dto.getGroupKey());
        entity.setSortOrder(dto.getSortOrder());
        entity.setDefaultQuantity(dto.getDefaultQuantity());
        entity.setDefaultUnitCode(dto.getDefaultUnitCode());
        entity.setDefaultDays(dto.getDefaultDays());
        entity.setFrequencyId(dto.getFrequencyId());
        entity.setRouteCode(dto.getRouteCode());
        entity.setDosageText(dto.getDosageText());
        entity.setDefaultPerformOrgId(dto.getDefaultPerformOrgId());
        entity.setLineRemark(dto.getLineRemark());
        entity.setChineseHerbsDoseQuantity(dto.getChineseHerbsDoseQuantity());
        entity.setSufferingFlag(dto.getSufferingFlag());
        entity.setUpdatedBy(userId);
        entity.setUpdatedAt(LocalDateTime.now());
    }

    public static OrderSetOperationVO toOperationVO(OrderSetDO entity, String message) {
        return OrderSetOperationVO.builder()
                .id(entity.getId())
                .status(entity.getStatusEnum())
                .published(OrderSetStatusEnum.ACTIVE.name().equals(entity.getStatusEnum()))
                .message(message)
                .version(entity.getVersion())
                .build();
    }
}
