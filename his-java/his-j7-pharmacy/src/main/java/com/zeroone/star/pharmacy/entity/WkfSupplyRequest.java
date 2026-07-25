package com.zeroone.star.pharmacy.entity;

import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Getter;
import lombok.Setter;

import java.io.Serializable;
import java.math.BigDecimal;
import java.time.LocalDateTime;

/**
 * <p>
 * 描述：供应请求实体类，对应数据库表 {@code wkf_supply_request}
 * </p>
 * <p>
 * 功能说明：记录供应请求信息（入库/出库/盘点/损益等），包含业务编号、
 * 供应类型、状态、物品表名和标识、数量、来源/目的库房、审批信息、
 * 批号、价格等。同一 bus_no 可聚合多行明细。
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 01star
 * @version 1.0.0
 */
@Getter
@Setter
@TableName("wkf_supply_request")
public class WkfSupplyRequest implements Serializable {

    private static final long serialVersionUID = 1L;

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

    /** 软删标记 */
    private Integer isDeleted;

    /** 删除时间 */
    private LocalDateTime deletedAt;

    /** 删除人 */
    private String deletedBy;

    /** 乐观锁版本号 */
    private Integer version;

    /** 供应请求聚合根（同表多行：同一 bus_no 聚合多行明细，共享审批状态） */
    private String busNo;

    /** 来源业务单号（如进货关联订货）；可空 */
    private String originalBusNo;

    /** 供应类型持久化码 */
    private String typeEnum;

    /** 供应状态持久化码 */
    private String statusEnum;

    /** intent枚举值 */
    private String intentEnum;

    /** 供应类别持久化码 */
    private String categoryEnum;

    /** priority枚举值 */
    private String priorityEnum;

    /** summary标识 */
    private String summaryId;

    /** deliver标识 */
    private String deliverId;

    /** 患者标识 */
    private String patientId;

    /** 项table */
    private String itemTable;

    /** 行数量：入出库等为执行数量（拆零、正数）；盘点/损益等为盈亏数量 */
    private BigDecimal itemQuantity;

    /** 合计/实盘数量：盘点场景存实盘数量（拆零、≥0） */
    private BigDecimal totalQuantity;

    /** 项标识 */
    private String itemId;

    /** 单元编码 */
    private String unitCode;

    /** occurrence时间 */
    private LocalDateTime occurrenceTime;

    /** 医护人员标识 */
    private String practitionerId;

    /** 供应商标识 */
    private String supplierId;

    /** 原因 */
    private String reason;

    /** 单库房单据为空；双端点单据必填（出方） */
    private String sourceTypeEnum;

    /** 单库房单据为空；双端点单据必填（出方） */
    private String sourceLocationId;

    /** 单库房单据必填；双端点单据必填 */
    private String purposeTypeEnum;

    /** 单库房就是目的；双端点为进方 */
    private String purposeLocationId;

    /** approver标识 */
    private String approverId;

    /** approval时间 */
    private LocalDateTime approvalTime;

    /** applicant标识 */
    private String applicantId;

    /** 提交审批时的申请时刻 */
    private LocalDateTime applyTime;

    /** 效期区间起点 */
    private LocalDateTime startTime;

    /** 效期区间止点 */
    private LocalDateTime endTime;

    /** 批号 */
    private String lotNumber;

    /** 追踪编号 */
    private String traceNo;

    /** 价格 */
    private BigDecimal price;

    /** 合计价格 */
    private BigDecimal totalPrice;

    /** 备注 */
    private String remake;

    /** 盘点盈亏金额；盘盈为正、盘亏为负；非盘点类型可空 */
    private BigDecimal profitLossPrice;
}
