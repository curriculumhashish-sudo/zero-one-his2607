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
 * 医保结算reversal
 * @TableName yb_settlement_reversal
 * @author wanfengzhiyunyi
 */
@TableName(value ="yb_settlement_reversal")
@Data
public class YbSettlementReversal implements Serializable {
    /**
     * 主键
     */
    @TableId
    private String id;

    /**
     * 结算记录标识
     */
    private String settlementRecordId;

    /**
     * revoke结算标识
     */
    private String revokeSetlId;

    /**
     * 就诊标识
     */
    private String mdtrtId;

    /**
     * psn编号
     */
    private String psnNo;

    /**
     * 就诊class
     */
    private String encounterClass;

    /**
     * 医疗费总额
     */
    private BigDecimal medfeeSumamt;

    /**
     * fund支付sumamt
     */
    private BigDecimal fundPaySumamt;

    /**
     * acct支付
     */
    private BigDecimal acctPay;

    /**
     * psncash支付
     */
    private BigDecimal psnCashPay;

    /**
     * 基金分项JSON
     */
    private String fundBreakdownJson;

    /**
     * reversed时间
     */
    private LocalDateTime reversedAt;

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
     * 租户标识
     */
    private String tenantId;

    /**
     * 调用日志标识
     */
    private String invokeLogId;

    @TableField(exist = false)
    private static final long serialVersionUID = 1L;
}