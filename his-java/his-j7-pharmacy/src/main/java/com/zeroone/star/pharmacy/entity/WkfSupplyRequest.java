package com.zeroone.star.pharmacy.entity;

import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableLogic;
import com.baomidou.mybatisplus.annotation.TableName;
import com.baomidou.mybatisplus.annotation.Version;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

import java.io.Serial;
import java.io.Serializable;
import java.math.BigDecimal;
import java.time.LocalDateTime;

/**
 * <p>
 * 描述：供应请求单实体类，对应数据库表 {@code wkf_supply_request}
 * </p>
 * <p>
 * 功能说明：该表为供应请求聚合根表，同一 {@code bus_no} 下聚合多条明细行，共享审批状态。
 * 在盘点单（{@code type_enum = INVENTORY_CHECK}）业务场景中：
 * - 一个盘点单号（{@code bus_no}）聚合多行药品明细
 * - {@code item_quantity} 表示盈亏数量（盘盈为正、盘亏为负）
 * - {@code total_quantity} 表示实盘数量
 * - {@code profit_loss_price} 表示盘点盈亏金额
 * 提交审批时，将 {@code status_enum} 由 {@code DRAFT} 变更为 {@code PENDING_APPROVAL}。
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 阿伟学长
 * @version 1.0.0
 */
@Getter
@Setter
@ToString
@TableName("wkf_supply_request")
public class WkfSupplyRequest implements Serializable {

    @Serial
    private static final long serialVersionUID = 1L;

    /**
     * 主键ID
     * 对应表字段：id
     */
    @TableId(value = "id", type = IdType.ASSIGN_ID)
    private String id;

    /**
     * 租户标识
     * 对应表字段：tenant_id
     */
    private String tenantId;

    /**
     * 创建时间
     * 对应表字段：created_at
     */
    private LocalDateTime createdAt;

    /**
     * 最后更新时间
     * 对应表字段：updated_at
     */
    private LocalDateTime updatedAt;

    /**
     * 创建人
     * 对应表字段：created_by
     */
    private String createdBy;

    /**
     * 最后更新人
     * 对应表字段：updated_by
     */
    private String updatedBy;

    /**
     * 软删标记
     * 对应表字段：is_deleted
     */
    @TableLogic
    private Boolean isDeleted;

    /**
     * 删除时间
     * 对应表字段：deleted_at
     */
    private LocalDateTime deletedAt;

    /**
     * 删除人
     * 对应表字段：deleted_by
     */
    private String deletedBy;

    /**
     * 乐观锁版本号
     * 对应表字段：version
     */
    @Version
    private Integer version;

    /**
     * 业务单号（聚合根）
     * 对应表字段：bus_no，同一单据号多行明细共享审批状态
     * 示例：PD20260720001
     */
    private String busNo;

    /**
     * 来源业务单号
     * 对应表字段：original_bus_no
     */
    private String originalBusNo;

    /**
     * 供应类型枚举
     * 对应表字段：type_enum
     * 盘点单固定为 INVENTORY_CHECK
     */
    private String typeEnum;

    /**
     * 供应状态枚举
     * 对应表字段：status_enum
     * DRAFT-草稿 PENDING_APPROVAL-待审批 APPROVED-已通过 REJECTED-已驳回 WITHDRAWN-已撤回
     */
    private String statusEnum;

    /**
     * 供应类别枚举
     * 对应表字段：category_enum
     * 盘点单为 INVENTORY
     */
    private String categoryEnum;

    /**
     * intent 枚举值
     * 对应表字段：intent_enum
     */
    private String intentEnum;

    /**
     * priority 枚举值
     * 对应表字段：priority_enum
     */
    private String priorityEnum;

    /**
     * summary 标识
     * 对应表字段：summary_id
     */
    private String summaryId;

    /**
     * deliver 标识
     * 对应表字段：deliver_id
     */
    private String deliverId;

    /**
     * 患者标识
     * 对应表字段：patient_id
     */
    private String patientId;

    /**
     * 项 table
     * 对应表字段：item_table
     */
    private String itemTable;

    /**
     * 行数量 / 盈亏数量
     * 对应表字段：item_quantity
     * 盘点场景：可正可负可零（盘盈为正、盘亏为负）
     */
    private BigDecimal itemQuantity;

    /**
     * 合计 / 实盘数量
     * 对应表字段：total_quantity
     * 盘点场景存实盘数量（≥0）
     */
    private BigDecimal totalQuantity;

    /**
     * 项标识
     * 对应表字段：item_id
     */
    private String itemId;

    /**
     * 单元编码
     * 对应表字段：unit_code
     */
    private String unitCode;

    /**
     * occurrence 时间
     * 对应表字段：occurrence_time
     */
    private LocalDateTime occurrenceTime;

    /**
     * 医护人员标识
     * 对应表字段：practitioner_id
     */
    private String practitionerId;

    /**
     * 供应商标识
     * 对应表字段：supplier_id
     */
    private String supplierId;

    /**
     * 原因
     * 对应表字段：reason
     */
    private String reason;

    /**
     * 来源类型枚举（双端点单据出方）
     * 对应表字段：source_type_enum
     */
    private String sourceTypeEnum;

    /**
     * 来源库房标识（双端点单据出方）
     * 对应表字段：source_location_id
     */
    private String sourceLocationId;

    /**
     * 目的类型枚举
     * 对应表字段：purpose_type_enum
     */
    private String purposeTypeEnum;

    /**
     * 目的库房标识
     * 对应表字段：purpose_location_id
     * 单库房单据为该库房；双端点单据为进方
     */
    private String purposeLocationId;

    /**
     * 审批人标识
     * 对应表字段：approver_id
     */
    private String approverId;

    /**
     * 审批时间
     * 对应表字段：approval_time
     */
    private LocalDateTime approvalTime;

    /**
     * 申请人标识
     * 对应表字段：applicant_id
     */
    private String applicantId;

    /**
     * 申请时间（提交审批时刻）
     * 对应表字段：apply_time
     * 草稿阶段可为空，提交时由服务端写入当前时间
     */
    private LocalDateTime applyTime;

    /**
     * 效期区间起点
     * 对应表字段：start_time
     */
    private LocalDateTime startTime;

    /**
     * 效期区间止点
     * 对应表字段：end_time
     */
    private LocalDateTime endTime;

    /**
     * 批号
     * 对应表字段：lot_number
     */
    private String lotNumber;

    /**
     * 追踪编号
     * 对应表字段：trace_no
     */
    private String traceNo;

    /**
     * 单价
     * 对应表字段：price
     */
    private BigDecimal price;

    /**
     * 合计价格
     * 对应表字段：total_price
     */
    private BigDecimal totalPrice;

    /**
     * 备注（注：数据库字段名为 remake）
     * 对应表字段：remake
     */
    private String remake;

    /**
     * 盘点盈亏金额
     * 对应表字段：profit_loss_price
     * 盘盈为正、盘亏为负；非盘点类型可空
     */
    private BigDecimal profitLossPrice;
}
