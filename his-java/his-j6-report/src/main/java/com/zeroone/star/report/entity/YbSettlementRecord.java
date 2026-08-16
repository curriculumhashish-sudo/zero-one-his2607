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
 * 医保 结算记录
 * @TableName yb_settlement_record
 * @author wanfengzhiyunyi
 */
@TableName(value ="yb_settlement_record")
@Data
public class YbSettlementRecord implements Serializable {
    /**
     * 主键
     */
    @TableId
    private String id;

    /**
     * 合同标识
     */
    private String contractId;

    /**
     * 就诊标识
     */
    private String encounterId;

    /**
     * 支付标识
     */
    private String paymentId;

    /**
     * presettle标识
     */
    private String preSettleId;

    /**
     * 收费批次号
     */
    private String chrgBchno;

    /**
     * 结算标识
     */
    private String setlId;

    /**
     * 就诊标识
     */
    private String mdtrtId;

    /**
     * psn编号
     */
    private String psnNo;

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
     * 账户标识
     */
    private String accountId;

    /**
     * 就诊class
     */
    private String encounterClass;

    /**
     * 状态
     */
    private String status;

    /**
     * 结算模式
     */
    private String settleMode;

    /**
     * 人员姓名
     */
    private String psnName;

    /**
     * 险种类型
     */
    private String insutype;

    /**
     * 医疗类别
     */
    private String medType;

    /**
     * 结算时间
     */
    private LocalDateTime setlTime;

    /**
     * 全自费金额
     */
    private BigDecimal fulamtOwnpayAmt;

    /**
     * 超限价自费费用
     */
    private BigDecimal overlmtSelfpay;

    /**
     * 先行自付金额
     */
    private BigDecimal preselfpayAmt;

    /**
     * 符合政策范围金额
     */
    private BigDecimal inscpScpAmt;

    /**
     * 基本医疗保险统筹基金支出
     */
    private BigDecimal hifpPay;

    /**
     * 公务员医疗补助资金支出
     */
    private BigDecimal cvlservPay;

    /**
     * 居民大病保险资金支出
     */
    private BigDecimal hifmiPay;

    /**
     * 职工大额医疗费用补助基金支出
     */
    private BigDecimal hifobPay;

    /**
     * 企业补充医疗保险基金支出
     */
    private BigDecimal hifesPay;

    /**
     * 公务员标志
     */
    private String cvlservFlag;

    /**
     * 基本医疗保险统筹基金支付比例
     */
    private BigDecimal poolPropSelfpay;

    /**
     * 医疗救助基金支出
     */
    private BigDecimal mafPay;

    /**
     * 其他支出
     */
    private BigDecimal othPay;

    /**
     * 个人负担总金额
     */
    private BigDecimal psnPartAmt;

    /**
     * 医院负担金额
     */
    private BigDecimal hospPartAmt;

    /**
     * 余额
     */
    private BigDecimal balc;

    /**
     * 个人账户共济支付金额
     */
    private BigDecimal acctMulaidPay;

    /**
     * 人员类别
     */
    private String psnType;

    /**
     * 实际支付起付线
     */
    private BigDecimal actPayDedc;

    /**
     * 伤残人员医疗保障基金支出
     */
    private BigDecimal hifdmPay;

    /**
     * 扩展内容JSON
     */
    private String expContent;

    /**
     * 医药机构结算ID
     */
    private String medinsSetlId;

    /**
     * 2207发送方报文ID
     */
    private String msgid2207;

    /**
     * 清算经办机构
     */
    private String clrOptins;

    /**
     * 清算方式
     */
    private String clrWay;

    /**
     * 清算类别
     */
    private String clrType;

    /**
     * 基金分项JSON
     */
    private String fundBreakdownJson;

    @TableField(exist = false)
    private static final long serialVersionUID = 1L;
}