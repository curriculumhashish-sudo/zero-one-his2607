package com.zeroone.star.basedata.entity.Medicine;

import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

import java.io.Serializable;
import java.math.BigDecimal;
import java.time.LocalDateTime;

/**
 * <p>
 * 管理域 收费项目定义
 * </p>
 *
 * @author xxm
 * @since 2026-07-19
 */
@Getter
@Setter
@ToString
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
    private LocalDateTime createdAt;

    /**
     * 最后更新时间
     */
    private LocalDateTime updatedAt;

    /**
     * 创建人
     */
    private String createdBy;

    /**
     * 最后更新人
     */
    private String updatedBy;

    /**
     * 软删标记
     */
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
     * 所属科室标识，可空表示全局价格
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
     * 价格数值（元，与 { adm_charge_item} 金额列同口径）
     */
    private BigDecimal priceValue;

    /**
     * 同实例多价时的优先级（数值越小越优先，由产品约定）
     */
    private Integer priority;
}
