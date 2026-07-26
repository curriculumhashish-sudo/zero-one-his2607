package com.zeroone.star.outpatient.entity;

import com.baomidou.mybatisplus.annotation.TableName;
import com.baomidou.mybatisplus.annotation.FieldFill;
import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableField;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableLogic;
import com.baomidou.mybatisplus.annotation.Version;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

import java.io.Serializable;
import java.math.BigDecimal;
import java.time.LocalDateTime;

/**
 * <p>
 * 财务 payment reconciliation
 * </p>
 *
 * @author Zzen
 * @since 2026-07-23
 */
@Getter
@Setter
@ToString
@TableName("fin_payment_reconciliation")
public class FinPaymentReconciliation implements Serializable {

    private static final long serialVersionUID = 1L;

    /**
     * 主键
     */
    @TableId(type = IdType.ASSIGN_UUID)
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
     * 软删标记
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
     * 支付流水聚合根
     */
    private String statusEnum;

    /**
     * relation 标识
     */
    private String relationId;

    /**
     * payment 编号
     */
    private String paymentNo;

    /**
     * payment 枚举值
     */
    private String paymentEnum;

    /**
     * kind 枚举值
     */
    private String kindEnum;

    /**
     * enterer 标识
     */
    private String entererId;

    /**
     * issuer 枚举值
     */
    private String issuerEnum;

    /**
     * 患者 标识
     */
    private String patientId;

    /**
     * practitioner 标识
     */
    private String practitionerId;

    /**
     * outcome 枚举值
     */
    private String outcomeEnum;

    /**
     * 地点 标识
     */
    private String locationId;

    /**
     * expiration日期
     */
    private LocalDateTime expirationDate;

    /**
     * tendered 金额（元，与库 numeric(19,6) 一致）
     */
    private BigDecimal tenderedAmount;

    /**
     * returned 金额
     */
    private BigDecimal returnedAmount;

    /**
     * display 金额
     */
    private BigDecimal displayAmount;

    /**
     * print 数量
     */
    private Integer printCount;

    /**
     * 合同 编号
     */
    private String contractNo;

    /**
     * 收费项ids
     */
    private String chargeItemIds;

    /**
     * 就诊 标识
     */
    private String encounterId;

    /**
     * bill日期
     */
    private LocalDateTime billDate;

    /**
     * 发票 标识
     */
    private String invoiceId;

    /**
     * 账号 标识
     */
    private String accountId;

    /**
     * ybClear 标志位
     */
    private Boolean ybClearFlag;

    /**
     * 收费员交账头 ID；非空表示已交账
     */
    private String cashierSettlementId;
}
