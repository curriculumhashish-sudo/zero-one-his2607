package com.zeroone.star.basedata.entity.equipment;

import com.baomidou.mybatisplus.annotation.*;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.math.BigDecimal;
import java.time.LocalDateTime;

@Data
@TableName("adm_charge_item_definition")
@Schema(description = "收费项目定义")
public class AdmChargeItemDefinitionDO {

    @TableId(type = IdType.ASSIGN_UUID)
    @Schema(description = "主键")
    private String id;

    @TableField("tenant_id")
    @Schema(description = "租户标识")
    private String tenantId;

    @TableField(value = "created_at", fill = FieldFill.INSERT)
    @Schema(description = "创建时间")
    private LocalDateTime createdAt;

    @TableField(value = "updated_at", fill = FieldFill.INSERT_UPDATE)
    @Schema(description = "最后更新时间")
    private LocalDateTime updatedAt;

    @TableField(value = "created_by", fill = FieldFill.INSERT)
    @Schema(description = "创建人")
    private String createdBy;

    @TableField(value = "updated_by", fill = FieldFill.INSERT_UPDATE)
    @Schema(description = "最后更新人")
    private String updatedBy;

    @TableField("is_deleted")
    @TableLogic(value = "0", delval = "1")
    @Schema(description = "软删标记")
    private Boolean isDeleted;

    @TableField("deleted_at")
    @Schema(description = "删除时间")
    private LocalDateTime deletedAt;

    @TableField("deleted_by")
    @Schema(description = "删除人")
    private String deletedBy;

    @Version
    @TableField("version")
    @Schema(description = "乐观锁版本号")
    private Integer version;

    @TableField("instance_table")
    @Schema(description = "实例来源表标识（如 adm_device_definition）")
    private String instanceTable;

    @TableField("instance_id")
    @Schema(description = "业务实例主键")
    private String instanceId;

    @TableField("charge_name")
    @Schema(description = "收费显示名称")
    private String chargeName;

    @TableField("status_enum")
    @Schema(description = "发布状态（ACTIVE/INACTIVE）")
    private String statusEnum;

    @TableField("org_id")
    @Schema(description = "所属机构主键")
    private String orgId;

    @TableField("type_code")
    @Schema(description = "收费类型编码（财务分类编码 fin_type_code）")
    private String typeCode;

    @TableField("yb_type")
    @Schema(description = "医保类别或对照编码")
    private String ybType;

    @TableField("price_type_code")
    @Schema(description = "价格类型编码（PURCHASE=采购价，SALE=零售价，LIMIT=限价）")
    private String priceTypeCode;

    @TableField("lot_no")
    @Schema(description = "批次或效期批次号")
    private String lotNo;

    @TableField("price_value")
    @Schema(description = "价格数值（元）")
    private BigDecimal priceValue;

    @TableField("priority")
    @Schema(description = "同实例多价时的优先级（数值越小越优先）")
    private Integer priority;
}