package com.zeroone.star.report.entity;


import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Data;

import java.io.Serializable;

import java.util.Date;
import java.math.BigDecimal;

/**
 * 管理域 收费项目
 * @TableName adm_charge_item
 */
@Data
@TableName("adm_charge_item")
public class AdmChargeItem implements Serializable {

    /**
     * 主键
     */
    @TableId(value = "id")
    private String id;
    /**
     * 租户标识
     */
    private String tenantId;
    /**
     * 创建时间
     */
    private Date createdAt;
    /**
     * 最后更新时间
     */
    private Date updatedAt;
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
    private Integer isDeleted;
    /**
     * 删除时间
     */
    private Date deletedAt;
    /**
     * 删除人
     */
    private String deletedBy;
    /**
     * 乐观锁版本号
     */
    private Integer version;
    /**
     * 生成来源存储码（DM generate_source_enum）
     */
    private String generateSourceEnum;
    /**
     * 收费上下文存储码（DM context_enum）
     */
    private String contextEnum;
    /**
     * 业务流水号（DM bus_no）
     */
    private String busNo;
    /**
     * 父收费明细主键（拆分/关联场景，DM parent_id）
     */
    private String parentId;
    /**
     * 处方号展示/追溯（DM prescription_no）
     */
    private String prescriptionNo;
    /**
     * 患者主键（DM 患者_id）
     */
    private String patientId;
    /**
     * 就诊主键（DM 就诊_id）
     */
    private String encounterId;
    /**
     * 上游业务表名锚点（BR-CHG-001，DM service_table）
     */
    private String serviceTable;
    /**
     * 上游业务主键锚点（DM service_id）
     */
    private String serviceId;
    /**
     * 账户主键（DM 账号_id）
     */
    private String accountId;
    /**
     * 收费项目定义主键（DM 定义_id）
     */
    private String definitionId;
    /**
     * 收费明细聚合根（DM-ADM-011 §3.1，表 adm_charge_item）
     */
    private String statusEnum;
    /**
     * 总价（DM total_price）
     */
    private BigDecimal totalPrice;
    /**
     * 费用发生时刻（DM occurrence_time）
     */
    private Date occurrenceTime;
    /**
     * 执行人主键（DM performer_id）
     */
    private String performerId;
    /**
     * 执行机构主键（DM performing_org_id）
     */
    private String performingOrgId;
    /**
     * 申请机构主键（DM requesting_org_id）
     */
    private String requestingOrgId;
    /**
     * 成本归属机构主键（DM cost_org_id）
     */
    private String costOrgId;
    /**
     * 数量值（DM quantity_value）
     */
    private BigDecimal quantityValue;
    /**
     * 数量单位（DM quantity_unit）
     */
    private String quantityUnit;
    /**
     * 单价（DM unit_price）
     */
    private BigDecimal unitPrice;
    /**
     * 定义明细主键（DM def_detail_id）
     */
    private String defDetailId;
    /**
     * 基准金额（DM base_amount）
     */
    private BigDecimal baseAmount;
    /**
     * 折扣金额（DM discount_amount）
     */
    private BigDecimal discountAmount;
    /**
     * 附加费金额（DM surcharge_amount）
     */
    private BigDecimal surchargeAmount;
    /**
     * 系统折后价（DM system_discount_price）
     */
    private BigDecimal systemDiscountPrice;
    /**
     * 手工调价后金额（DM manual_adjusted_price）
     */
    private BigDecimal manualAdjustedPrice;
    /**
     * 改价原因码（DM override_reason_code）
     */
    private String overrideReasonCode;
    /**
     * 改价原因说明（DM override_reason_text）
     */
    private String overrideReasonText;
    /**
     * 录入人主键（DM enterer_id）
     */
    private String entererId;
    /**
     * 录入时间（DM entered_date）
     */
    private Date enteredDate;
    /**
     * 产品来源表名（DM product_table）
     */
    private String productTable;
    /**
     * 产品主键（DM product_id）
     */
    private String productId;
    /**
     * 处置/手术类关联主键（DM 手术/操作_id）
     */
    private String procedureId;
    /**
     * 发药来源表名（DM dispense_table）
     */
    private String dispenseTable;
    /**
     * 发药主键（DM dispense_id）
     */
    private String dispenseId;
    /**
     * 理赔状态枚举（DM claim_state_enum）
     */
    private Integer claimStateEnum;
    /**
     * 打印次数（DM print_count）
     */
    private Integer printCount;
    /**
     * 开立机构主键（DM org_id）
     */
    private String orgId;
    /**
     * 退费关联主键（DM refund_id）
     */
    private String refundId;
    /**
     * 病情/诊断条件关联（DM 病情_id）
     */
    private String conditionId;
    /**
     * 就诊诊断关联主键（DM 就诊_diagnosis_id）
     */
    private String encounterDiagnosisId;
    /**
     * 是否中医相关标记（DM tcm_flag）
     */
    private Integer tcmFlag;

}
