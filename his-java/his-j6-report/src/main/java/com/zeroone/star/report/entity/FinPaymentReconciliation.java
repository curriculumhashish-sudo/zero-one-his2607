package com.zeroone.star.report.entity;

import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableField;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;
import java.io.Serializable;
import java.math.BigDecimal;
import java.time.LocalDateTime;
import lombok.Data;

/**
 * 财务 payment reconciliation
 * @TableName fin_payment_reconciliation
 * @author wanfengzhiyunyi
 */
@TableName(value ="fin_payment_reconciliation")
@Data
public class FinPaymentReconciliation implements Serializable {
    /**
     * 主键
     */
    @TableId
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
    private Integer isDeleted;

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
    private Integer ybClearFlag;

    /**
     * 收费员交账头 ID；非空表示已交账
     */
    private String cashierSettlementId;

    @TableField(exist = false)
    private static final long serialVersionUID = 1L;
}