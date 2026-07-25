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
 * 财务 payment rec detail
 * </p>
 *
 * @author Zzen
 * @since 2026-07-23
 */
@Getter
@Setter
@ToString
@TableName("fin_payment_rec_detail")
public class FinPaymentRecDetail implements Serializable {

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
     * 对账标识
     */
    private String reconciliationId;

    /**
     * predecessor标识
     */
    private String predecessorId;

    /**
     * target枚举值
     */
    private String targetEnum;

    /**
     * 账户标识
     */
    private String accountId;

    /**
     * 账户编码
     */
    private String accountCode;

    /**
     * 支付枚举值
     */
    private String payEnum;

    /**
     * 支付level枚举值
     */
    private String payLevelEnum;

    /**
     * 金额
     */
    private BigDecimal amount;

    /**
     * return金额
     */
    private BigDecimal returnAmount;

    /**
     * 收费金额
     */
    private BigDecimal chargeAmount;

    /**
     * 支付trans编号
     */
    private String payTransNo;

    /**
     * 支付trans文本
     */
    private String payTransText;

    /**
     * 支付trans日期
     */
    private LocalDateTime payTransDate;

    /**
     * before余额
     */
    private BigDecimal beforeBalance;

    /**
     * after余额
     */
    private BigDecimal afterBalance;

    /**
     * result枚举值
     */
    private String resultEnum;
}
