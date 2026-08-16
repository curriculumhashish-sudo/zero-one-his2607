package com.zeroone.star.dispense.entity;

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
 * 药房盘点单，对应工作流供应请求表中的 PHARMACY_STOCKTAKE 单据。
 *
 * @author j7
 * @since 2026-07-26
 */
@Getter
@Setter
@ToString
@TableName("wkf_supply_request")
public class StockTakeOrder implements Serializable {

    @Serial
    private static final long serialVersionUID = 1L;

    /** 主键 */
    @TableId(value = "id", type = IdType.ASSIGN_ID)
    private String id;

    /** 租户标识 */
    private String tenantId;

    /** 创建时间 */
    private LocalDateTime createdAt;

    /** 最后更新时间 */
    private LocalDateTime updatedAt;

    /** 创建人 */
    private String createdBy;

    /** 最后更新人 */
    private String updatedBy;

    /** 软删除标记 */
    @TableLogic
    private Boolean isDeleted;

    /** 删除时间 */
    private LocalDateTime deletedAt;

    /** 删除人 */
    private String deletedBy;

    /** 乐观锁版本号 */
    @Version
    private Integer version;

    /** 盘点单号，同一单号可聚合多行药品明细 */
    private String busNo;

    /** 来源业务单号 */
    private String originalBusNo;

    /** 供应类型，药房盘点固定为 PHARMACY_STOCKTAKE */
    private String typeEnum;

    /** 单据状态 */
    private String statusEnum;

    /** 业务意图 */
    private String intentEnum;

    /** 供应类别 */
    private String categoryEnum;

    /** 优先级 */
    private String priorityEnum;

    /** 汇总记录标识 */
    private String summaryId;

    /** 发放记录标识 */
    private String deliverId;

    /** 患者标识 */
    private String patientId;

    /** 盘点项目所在表 */
    private String itemTable;

    /** 盘盈盘亏数量，可正、可负或为零 */
    private BigDecimal itemQuantity;

    /** 实盘数量 */
    private BigDecimal totalQuantity;

    /** 盘点项目标识 */
    private String itemId;

    /** 数量单位编码 */
    private String unitCode;

    /** 业务发生时间 */
    private LocalDateTime occurrenceTime;

    /** 医护人员标识 */
    private String practitionerId;

    /** 供应商标识 */
    private String supplierId;

    /** 盘点原因 */
    private String reason;

    /** 来源位置类型 */
    private String sourceTypeEnum;

    /** 来源位置标识 */
    private String sourceLocationId;

    /** 盘点位置类型，药房盘点为 PHARMACY */
    private String purposeTypeEnum;

    /** 盘点药房标识 */
    private String purposeLocationId;

    /** 审批人标识 */
    private String approverId;

    /** 审批时间 */
    private LocalDateTime approvalTime;

    /** 申请人标识 */
    private String applicantId;

    /** 提交审批时间 */
    private LocalDateTime applyTime;

    /** 效期起点 */
    private LocalDateTime startTime;

    /** 效期终点 */
    private LocalDateTime endTime;

    /** 批号 */
    private String lotNumber;

    /** 追溯码 */
    private String traceNo;

    /** 单价 */
    private BigDecimal price;

    /** 实盘库存金额 */
    private BigDecimal totalPrice;

    /** 备注，对应数据库 remake 字段 */
    private String remake;

    /** 盘点盈亏金额，盘盈为正、盘亏为负 */
    private BigDecimal profitLossPrice;
}
