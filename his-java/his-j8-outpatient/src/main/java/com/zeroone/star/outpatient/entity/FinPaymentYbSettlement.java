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
import java.time.LocalDateTime;

/**
 * <p>
 * 支付流水关联的医保正式结算凭证（一行一个 setl_id）
 * </p>
 *
 * @author Zzen
 * @since 2026-07-23
 */
@Getter
@Setter
@ToString
@TableName("fin_payment_yb_settlement")
public class FinPaymentYbSettlement implements Serializable {

    private static final long serialVersionUID = 1L;

    /**
     * 主键
     */
    @TableId(type = IdType.ASSIGN_UUID)
    private String id;

    /**
     * 对账标识
     */
    private String reconciliationId;

    /**
     * 平台 setl_id
     */
    private String setlId;

    /**
     * 收费批次号
     */
    private String chrgBchno;

    /**
     * 就诊标识
     */
    private String mdtrtId;

    /**
     * yb_settlement_record.id（CHS 侧）
     */
    private String settlementRecordId;

    /**
     * 账户标识
     */
    private String accountId;

    /**
     * 状态
     */
    private String status;

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
     * 租户标识
     */
    private String tenantId;
}
