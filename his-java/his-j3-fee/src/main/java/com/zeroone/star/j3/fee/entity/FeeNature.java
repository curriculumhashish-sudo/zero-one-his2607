package com.zeroone.star.j3.fee.entity;

import com.baomidou.mybatisplus.annotation.*;
import lombok.Getter;
import lombok.Setter;

import java.io.Serializable;
import java.time.LocalDateTime;

/**
 * <p>
 * 费用性质实体（映射 fin_contract 表）
 * </p>
 * @author 白文儿
 * @version 1.0.0
 */
@Getter
@Setter
@TableName("fin_contract")
public class FeeNature implements Serializable {

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
     * 费用性质编码（业务编号）
     */
    private String busNo;

    /**
     * 费用性质名称
     */
    private String name;

    /**
     * 费用性质分类（0自费、1公费、2省医保、3市医保）
     */
    private String categoryEnum;

    /**
     * 状态枚举值
     */
    private String statusEnum;

    /**
     * 是否启用 1 启用 0 禁用
     */
    private Integer activeFlag;

    /**
     * 显示顺序
     */
    private Integer displayOrder;

    /**
     * 医保标志位
     */
    private Integer ybFlag;

    /**
     * 机构标识
     */
    private String orgId;
}
