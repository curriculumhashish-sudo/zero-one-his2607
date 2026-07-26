package com.zeroone.star.j3.fee.entity;

import com.baomidou.mybatisplus.annotation.*;
import lombok.Getter;
import lombok.Setter;

import java.io.Serializable;
import java.math.BigDecimal;
import java.time.LocalDateTime;

/**
 * <p>
 * 管理域账号实体（映射 adm_account 表）
 * </p>
 * @author 白文儿
 * @version 1.0.0
 */
@Getter
@Setter
@TableName("adm_account")
public class AdmAccount implements Serializable {

    private static final long serialVersionUID = 1L;

    /**
     * 主键（UUID）
     */
    @TableId(value = "id", type = IdType.ASSIGN_UUID)
    private String id;

    // ========== 审计字段 ==========

    @TableField(fill = FieldFill.INSERT)
    private String tenantId;

    @TableField(fill = FieldFill.INSERT)
    private LocalDateTime createdAt;

    @TableField(fill = FieldFill.INSERT_UPDATE)
    private LocalDateTime updatedAt;

    @TableField(fill = FieldFill.INSERT)
    private String createdBy;

    @TableField(fill = FieldFill.INSERT_UPDATE)
    private String updatedBy;

    @TableLogic
    private Integer isDeleted;

    private LocalDateTime deletedAt;

    private String deletedBy;

    @Version
    private Integer version;

    // ========== 业务字段 ==========

    /**
     * 就诊账户聚合根：与单次就诊关联的分账账户（如自费/医保等）
     */
    private String patientId;

    /**
     * 就诊标识
     */
    private String encounterId;

    /**
     * type 代码
     */
    private String typeCode;

    /**
     * 业务流水号（新建必填）
     */
    private String busNo;

    /**
     * 名称
     */
    private String name;

    /**
     * 余额
     */
    private BigDecimal balance;

    /**
     * 结账状态（枚举值）
     */
    private String billingStatusEnum;

    /**
     * 合同编号
     */
    private String contractNo;

    /**
     * ybArea 编号
     */
    private String ybAreaNo;

    /**
     * 就诊标志位
     */
    private Integer encounterFlag;

    /**
     * 险种类型
     */
    private String insutype;

    /**
     * 默认标志位
     */
    private Integer defaultFlag;

    /**
     * 账户主状态（有效/无效，枚举值）
     */
    private String statusEnum;
}