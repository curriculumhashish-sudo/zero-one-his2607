package com.zeroone.star.pharmacy.entity;

import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

import java.io.Serial;
import java.io.Serializable;
import java.math.BigDecimal;
import java.time.LocalDateTime;

/**
 * <p>
 * 工作流 supply delivery
 * </p>
 *
 * @author leisurely
 * @since 2026-07-24
 */
@Getter
@Setter
@ToString
@TableName("wkf_supply_delivery")
public class WkfSupplyDelivery implements Serializable {

    @Serial
    private static final long serialVersionUID = 1L;

    /**
     * 主键
     */
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
     * 供应发放聚合根
     */
    private String requestId;

    /**
     * summary标识
     */
    private String summaryId;

    /**
     * 发放状态持久化码（如 { DispenseStatus#getCode()}，B.10.8.1）
     */
    private String statusEnum;

    /**
     * 患者标识
     */
    private String patientId;

    /**
     * 发放类型/业务子类型持久化码（与命令及事件载荷一致，B.10.8.1）
     */
    private String typeEnum;

    /**
     * 项table
     */
    private String itemTable;

    /**
     * 项标识
     */
    private String itemId;

    /**
     * 依据表名
     */
    private String basedOnTable;

    /**
     * 依据ID列表
     */
    private String basedOnIds;

    /**
     * 单元编码
     */
    private String unitCode;

    /**
     * 数量
     */
    private BigDecimal quantity;

    /**
     * 病情
     */
    private String condition;

    /**
     * 批号
     */
    private String lotNumber;

    /**
     * 追踪编号
     */
    private String traceNo;

    /**
     * 供应商标识
     */
    private String supplierId;

    /**
     * 医护人员标识
     */
    private String practitionerId;

    /**
     * occurrence时间
     */
    private LocalDateTime occurrenceTime;

    /**
     * receiver标识
     */
    private String receiverId;

    /**
     * receive时间
     */
    private LocalDateTime receiveTime;
}
