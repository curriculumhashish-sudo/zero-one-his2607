package com.zeroone.star.hisj5basedata.entity;

import com.baomidou.mybatisplus.annotation.*;
import lombok.Data;
import lombok.NoArgsConstructor;
import lombok.AllArgsConstructor;

import java.io.Serializable;
import java.math.BigDecimal;
import java.time.LocalDateTime;

/**
 * <p>
 * 组套模板明细行（映射 cus_order_template_line）
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author keepasdd
 * @version 1.0.0
 */
@Data
@NoArgsConstructor
@AllArgsConstructor
@TableName("cus_order_template_line")
public class OrderTemplateLine implements Serializable {

    private static final long serialVersionUID = 1L;

    /** 主键 */
    @TableId(value = "id", type = IdType.ASSIGN_ID)
    private String id;

    /** 组套模板头 ID */
    @TableField("order_template_id")
    private String orderTemplateId;

    /** 行类型枚举 */
    @TableField("line_kind_enum")
    private String lineKindEnum;

    /** 药品定义 ID */
    @TableField("medication_definition_id")
    private String medicationDefinitionId;

    /** 诊疗活动定义 ID */
    @TableField("activity_definition_id")
    private String activityDefinitionId;

    /** 耗材设备定义 ID */
    @TableField("device_definition_id")
    private String deviceDefinitionId;

    /** 分组 key */
    @TableField("group_key")
    private String groupKey;

    /** 排序号 */
    @TableField("sort_order")
    private Integer sortOrder;

    /** 默认数量 */
    @TableField("default_quantity")
    private BigDecimal defaultQuantity;

    /** 默认单位编码 */
    @TableField("default_unit_code")
    private String defaultUnitCode;

    /** 默认天数 */
    @TableField("default_days")
    private Integer defaultDays;

    /** 频次 ID */
    @TableField("frequency_id")
    private String frequencyId;

    /** 给药途径编码 */
    @TableField("route_code")
    private String routeCode;

    /** 剂量说明 */
    @TableField("dosage_text")
    private String dosageText;

    /** 默认执行科室 ID */
    @TableField("default_perform_org_id")
    private String defaultPerformOrgId;

    /** 行备注 */
    @TableField("line_remark")
    private String lineRemark;

    /** TCM 付数 */
    @TableField("chinese_herbs_dose_quantity")
    private BigDecimal chineseHerbsDoseQuantity;

    /** TCM 代煎标志 */
    @TableField("suffering_flag")
    private Boolean sufferingFlag;

    // ========== 审计字段 ==========

    /** 租户标识 */
    @TableField("tenant_id")
    private String tenantId;

    /** 创建时间 */
    @TableField(value = "created_at", fill = FieldFill.INSERT)
    private LocalDateTime createdAt;

    /** 最后更新时间 */
    @TableField(value = "updated_at", fill = FieldFill.INSERT_UPDATE)
    private LocalDateTime updatedAt;

    /** 创建人 */
    @TableField(value = "created_by", fill = FieldFill.INSERT)
    private String createdBy;

    /** 最后更新人 */
    @TableField(value = "updated_by", fill = FieldFill.INSERT_UPDATE)
    private String updatedBy;

    /** 软删标记 */
    @TableLogic
    @TableField("is_deleted")
    private Boolean isDeleted;

    /** 删除时间 */
    @TableField("deleted_at")
    private LocalDateTime deletedAt;

    /** 删除人 */
    @TableField("deleted_by")
    private String deletedBy;

    /** 乐观锁版本号 */
    @Version
    @TableField("version")
    private Integer version;
}
