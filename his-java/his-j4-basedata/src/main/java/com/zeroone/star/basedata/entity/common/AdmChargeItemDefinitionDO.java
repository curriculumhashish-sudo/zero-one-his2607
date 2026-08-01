package com.zeroone.star.basedata.entity.common;

import com.baomidou.mybatisplus.annotation.*;
import lombok.Data;
import lombok.EqualsAndHashCode;

import java.io.Serializable;
import java.math.BigDecimal;
import java.time.LocalDateTime;

/**
 * <p>
 * 描述：管理域收费项目定义实体，对应 adm_charge_item_definition 表
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@Data
@EqualsAndHashCode(callSuper = false)
@TableName("adm_charge_item_definition")
public class AdmChargeItemDefinitionDO implements Serializable {

    private static final long serialVersionUID = 1L;

    /**
     * 主键
     */
    @TableId(value = "id", type = IdType.ASSIGN_ID)
    private String id;

    /**
     * 租户标识
     */
    private String tenantId;

    /**
     * 创建时间
     */
    @TableField(fill = FieldFill.INSERT)
    private LocalDateTime createdAt;

    /**
     * 最后更新时间
     */
    @TableField(fill = FieldFill.INSERT_UPDATE)
    private LocalDateTime updatedAt;

    /**
     * 创建人
     */
    @TableField(fill = FieldFill.INSERT)
    private String createdBy;

    /**
     * 最后更新人
     */
    @TableField(fill = FieldFill.INSERT_UPDATE)
    private String updatedBy;

    /**
     * 软删标记 (0=未删除, 1=已删除)
     */
    @TableLogic
    private Boolean isDeleted;

    /**
     * 删除时间
     */
    private LocalDateTime deletedAt;

    /**
     * 删除人
     */
    private String deletedBy;

    /**
     * 乐观锁版本号
     */
    @Version
    private Integer version;

    /**
     * 收费项目定义实体：描述某业务实例在特定价类/批次下的可计费项与价格
     */
    private String instanceTable;

    /**
     * 业务实例主键
     */
    private String instanceId;

    /**
     * 收费展示名称
     */
    private String chargeName;

    /**
     * 状态枚举编码（PublicationStatus，默认 ACTIVE）
     */
    private String statusEnum;

    /**
     * 所属机构主键（可空表示全局）
     */
    private String orgId;

    /**
     * 收费类型编码（fin_type_code，必填；号源落诊察费 1012）
     */
    private String typeCode;

    /**
     * 医保类别或对照编码
     */
    private String ybType;

    /**
     * 价格类型编码（必填）
     */
    private String priceTypeCode;

    /**
     * 批次或效期批次号
     */
    private String lotNo;

    /**
     * 价格数值（元，与 adm_charge_item 金额列同口径）
     */
    private BigDecimal priceValue;

    /**
     * 同实例多价时的优先级（数值越小越优先，由产品约定）
     */
    private Integer priority;
}