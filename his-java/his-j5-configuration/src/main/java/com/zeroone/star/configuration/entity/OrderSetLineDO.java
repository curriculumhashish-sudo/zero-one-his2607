package com.zeroone.star.configuration.entity;

import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableLogic;
import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Data;

import java.math.BigDecimal;
import java.time.LocalDateTime;

/**
 * 组套模板明细数据库对象，对应 {@code cus_order_template_line}。
 */
@Data
@TableName("cus_order_template_line")
public class OrderSetLineDO {

    @TableId(value = "id", type = IdType.ASSIGN_ID)
    private String id;
    private String tenantId;
    private LocalDateTime createdAt;
    private LocalDateTime updatedAt;
    private String createdBy;
    private String updatedBy;
    @TableLogic(value = "0", delval = "1")
    private Boolean isDeleted;
    private LocalDateTime deletedAt;
    private String deletedBy;
    private Integer version;
    private String orderTemplateId;
    private String lineKindEnum;
    private String medicationDefinitionId;
    private String activityDefinitionId;
    private String deviceDefinitionId;
    private String groupKey;
    private Integer sortOrder;
    private BigDecimal defaultQuantity;
    private String defaultUnitCode;
    private Integer defaultDays;
    private String frequencyId;
    private String routeCode;
    private String dosageText;
    private String defaultPerformOrgId;
    private String lineRemark;
    private BigDecimal chineseHerbsDoseQuantity;
    private Boolean sufferingFlag;
}
