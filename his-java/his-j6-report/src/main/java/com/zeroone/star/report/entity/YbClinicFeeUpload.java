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
 * 2204 门诊费用上传及平台分割结果
 * @TableName yb_clinic_fee_upload
 * @author wanfengzhiyunyi
 */
@TableName(value ="yb_clinic_fee_upload")
@Data
public class YbClinicFeeUpload implements Serializable {
    /**
     * 主键
     */
    @TableId
    private String id;

    /**
     * 就诊标识
     */
    private String encounterId;

    /**
     * 收费项标识
     */
    private String chargeItemId;

    /**
     * 费用明细流水号
     */
    private String feedetlSn;

    /**
     * 就诊标识
     */
    private String mdtrtId;

    /**
     * 收费批次号
     */
    private String chrgBchno;

    /**
     * 预结算标识
     */
    private String preSettleId;

    /**
     * 上传状态
     */
    private String uploadStatus;

    /**
     * 明细项目费用总额
     */
    private BigDecimal detItemFeeSumamt;

    /**
     * 收费项目等级
     */
    private String chrgitmLv;

    /**
     * 医疗收费项目类别
     */
    private String medChrgitmType;

    /**
     * 自付比例
     */
    private BigDecimal selfpayProp;

    /**
     * 全自费金额
     */
    private BigDecimal fulamtOwnpayAmt;

    /**
     * 超限价金额
     */
    private BigDecimal overlmtAmt;

    /**
     * 先行自付金额
     */
    private BigDecimal preselfpayAmt;

    /**
     * 符合政策范围金额
     */
    private BigDecimal inscpScpAmt;

    /**
     * 限制使用标志
     */
    private String lmtUsedFlag;

    /**
     * 平台备注
     */
    private String platformMemo;

    /**
     * 平台分割结果JSON
     */
    private String platformResultJson;

    /**
     * 上传时间
     */
    private LocalDateTime uploadedAt;

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

    @TableField(exist = false)
    private static final long serialVersionUID = 1L;
}