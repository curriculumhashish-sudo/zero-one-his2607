package com.zeroone.star.pharmacy.entity;

import com.baomidou.mybatisplus.annotation.*;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

import java.io.Serial;
import java.io.Serializable;
import java.math.BigDecimal;
import java.time.LocalDateTime;

/**
 * <p>
 *     描述：药房入库单实体类，对应数据库表 {@code wkf_supply_request}
 * </p>
 * <p>
 *     功能说明：该表为供应请求聚合根表，同一 {@code bus_no} 下聚合多条明细行，共享审批状态。
 *     支持采购入库、调拨入库等多种业务场景。
 * </p>
 */
@Getter
@Setter
@ToString
@TableName("wkf_supply_request")
public class InStockOrder implements Serializable {

    @Serial
    private static final long serialVersionUID = 1L;

    /**
     * 主键ID
     */
    @TableId(value = "id", type = IdType.ASSIGN_ID)
    private String id;

    /**
     * 租户标识
     */
    @TableField("tenant_id")
    private String tenantId;

    /**
     * 创建时间
     */
    @TableField("created_at")
    private LocalDateTime createdAt;

    /**
     * 最后更新时间
     */
    @TableField("updated_at")
    private LocalDateTime updatedAt;

    /**
     * 创建人ID (对应UI: 制单人)
     */
    @TableField("created_by")
    private String createdBy;

    /**
     * 最后更新人ID
     */
    @TableField("updated_by")
    private String updatedBy;

    /**
     * 软删标记 (0-未删除, 1-已删除)
     */
    @TableLogic
    @TableField("is_deleted")
    private Boolean isDeleted;

    /**
     * 删除时间
     */
    @TableField("deleted_at")
    private LocalDateTime deletedAt;

    /**
     * 删除人ID
     */
    @TableField("deleted_by")
    private String deletedBy;

    /**
     * 乐观锁版本号
     */
    @Version
    @TableField("version")
    private Integer version;

    /**
     * 业务单号 (聚合根)
     * 对应UI: PSI2026-06-300001
     * 说明：同一单据号多行明细共享审批状态
     */
    @TableField("bus_no")
    private String busNo;

    /**
     * 来源业务单号
     * 说明：如进货关联订货单号；可空
     */
    @TableField("original_bus_no")
    private String originalBusNo;

    /**
     * 供应类型枚举
     * 说明：区分是采购入库、调拨入库还是其他类型
     */
    @TableField("type_enum")
    private String typeEnum;

    /**
     * 供应状态枚举
     * 对应UI: 同意 (APPROVED) / 待审核 (PENDING)
     */
    @TableField("status_enum")
    private String statusEnum;

    /**
     * 意图枚举值
     */
    @TableField("intent_enum")
    private String intentEnum;

    /**
     * 供应类别枚举
     */
    @TableField("category_enum")
    private String categoryEnum;

    /**
     * 优先级枚举值
     */
    @TableField("priority_enum")
    private String priorityEnum;

    /**
     * Summary 标识
     */
    @TableField("summary_id")
    private String summaryId;

    /**
     * Deliver 标识
     */
    @TableField("deliver_id")
    private String deliverId;

    /**
     * 患者标识
     */
    @TableField("patient_id")
    private String patientId;

    /**
     * 项 Table 名称
     */
    @TableField("item_table")
    private String itemTable;

    /**
     * 行数量 / 执行数量
     * 说明：入出库等为执行数量（拆零、正数）
     */
    @TableField("item_quantity")
    private BigDecimal itemQuantity;

    /**
     * 合计数量 / 实盘数量
     * 说明：盘点场景存实盘数量；其它单据按业务口径使用
     */
    @TableField("total_quantity")
    private BigDecimal totalQuantity;

    /**
     * 药品/物资项标识
     */
    @TableField("item_id")
    private String itemId;

    /**
     * 单元编码 (单位)
     */
    @TableField("unit_code")
    private String unitCode;

    /**
     * 发生时间 (Occurrence Time)
     */
    @TableField("occurrence_time")
    private LocalDateTime occurrenceTime;

    /**
     * 医护人员/操作员标识
     */
    @TableField("practitioner_id")
    private String practitionerId;

    /**
     * 供应商标识
     * 对应UI: 供应商名称 (需关联查询)
     */
    @TableField("supplier_id")
    private String supplierId;

    /**
     * 原因
     */
    @TableField("reason")
    private String reason;

    /**
     * 来源类型枚举 (双端点单据出方)
     * 对应UI: 西药库 (调拨时的出库方)
     */
    @TableField("source_type_enum")
    private String sourceTypeEnum;

    /**
     * 来源库房标识 (双端点单据出方)
     * 对应UI: 西药库
     */
    @TableField("source_location_id")
    private String sourceLocationId;

    /**
     * 目的类型枚举
     * 对应UI: 门诊西药房 (调拨时的入库方)
     */
    @TableField("purpose_type_enum")
    private String purposeTypeEnum;

    /**
     * 目的库房标识
     * 对应UI: 门诊西药房
     * 说明：单库房单据为该库房；双端点单据为进方
     */
    @TableField("purpose_location_id")
    private String purposeLocationId;

    /**
     * 审批人标识
     * 对应UI: 审批人
     */
    @TableField("approver_id")
    private String approverId;

    /**
     * 审批时间
     */
    @TableField("approval_time")
    private LocalDateTime approvalTime;

    /**
     * 申请人标识
     */
    @TableField("applicant_id")
    private String applicantId;

    /**
     * 申请时间 (提交审批时刻)
     * 对应UI: 2026-06-30 16:35
     */
    @TableField("apply_time")
    private LocalDateTime applyTime;

    /**
     * 效期区间起点
     */
    @TableField("start_time")
    private LocalDateTime startTime;

    /**
     * 效期区间止点
     */
    @TableField("end_time")
    private LocalDateTime endTime;

    /**
     * 批号
     */
    @TableField("lot_number")
    private String lotNumber;

    /**
     * 追踪编号
     */
    @TableField("trace_no")
    private String traceNo;

    /**
     * 单价
     */
    @TableField("price")
    private BigDecimal price;

    /**
     * 合计价格
     * 对应UI: 总金额
     */
    @TableField("total_price")
    private BigDecimal totalPrice;

    /**
     * 备注
     * 注意：数据库字段名为 remake
     */
    @TableField("remake")
    private String remake;

    /**
     * 盘点盈亏金额
     * 说明：盘盈为正、盘亏为负；非盘点类型可空
     */
    @TableField("profit_loss_price")
    private BigDecimal profitLossPrice;
}