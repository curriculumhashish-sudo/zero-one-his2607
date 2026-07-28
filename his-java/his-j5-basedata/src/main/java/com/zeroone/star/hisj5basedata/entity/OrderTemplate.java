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
 * 组套模板头（映射 cus_order_template）
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author keepasdd
 * @version 1.0.0
 */
@Data
@NoArgsConstructor
@AllArgsConstructor
@TableName("cus_order_template")
public class OrderTemplate implements Serializable {

    private static final long serialVersionUID = 1L;

    /** 主键 */
    @TableId(value = "id", type = IdType.ASSIGN_ID)
    private String id;

    /** 组套模板头聚合根 */
    @TableField("object_id")
    private String objectId;

    /** 绑定类型（PERSONAL/DEPT/GLOBAL） */
    @TableField("binding_enum")
    private String bindingEnum;

    /** 适用场景（OUTPATIENT/INPATIENT/GENERAL） */
    @TableField("scene_enum")
    private String sceneEnum;

    /** 模板编码 */
    @TableField("template_code")
    private String templateCode;

    /** 模板名称 */
    @TableField("name")
    private String name;

    /** 描述 */
    @TableField("description")
    private String description;

    /** 列表排序权重 */
    @TableField("sort_weight")
    private Integer sortWeight;

    /** 状态（DRAFT/ACTIVE/INACTIVE） */
    @TableField("status_enum")
    private String statusEnum;

    /** 备注 */
    @TableField("remark")
    private String remark;

    /** 组套种类（GENERAL/TCM/PRICING） */
    @TableField("template_kind_enum")
    private String templateKindEnum;

    /** TCM 组套默认中药付数 */
    @TableField("default_chinese_herbs_dose_quantity")
    private BigDecimal defaultChineseHerbsDoseQuantity;

    /** TCM 组套默认代煎标志 */
    @TableField("default_suffering_flag")
    private Boolean defaultSufferingFlag;

    /** 模板来源（MAINTAINED/IMPORTED） */
    @TableField("template_source_enum")
    private String templateSourceEnum;

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
