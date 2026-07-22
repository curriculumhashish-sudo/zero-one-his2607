package com.zeroone.star.j7.pharmacy.entity;

import com.baomidou.mybatisplus.annotation.FieldFill;
import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableField;
import com.baomidou.mybatisplus.annotation.TableId;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

import java.io.Serializable;
import java.time.LocalDateTime;

/**
 * <p>
 * 药房入库单主表
 * </p>
 *
 * @author wl
 * @since 2026-07-22
 */
@Getter
@Setter
@ToString
public class Pharmacy implements Serializable {

    private static final long serialVersionUID = 1L;

    /**
     * 主键
     */
    @TableId(value = "id", type = IdType.AUTO)
    private Long id;

    /**
     * 单据号，如PSI2026-07-22001
     */
    private String billNo;

    /**
     * 单据状态，如同意
     */
    private String status;

    /**
     * 源药房编码
     */
    private String sourcePharmacyCode;

    /**
     * 源药房名称
     */
    private String sourcePharmacyName;

    /**
     * 目的药房编码
     */
    private String targetPharmacyCode;

    /**
     * 目的药房，如门诊西药房
     */
    private String targetPharmacyName;

    /**
     * 项目类型，药品
     */
    private String itemType;

    /**
     * 关联请领单号
     */
    private String relReqNo;

    /**
     * 创建时间
     */
    @TableField(fill = FieldFill.INSERT)
    private LocalDateTime createTime;

    /**
     * 主表备注
     */
    private String remark;

    /**
     * 创建人
     */
    @TableField(fill = FieldFill.INSERT)
    private String createBy;

    /**
     * 更新人
     */
    @TableField(fill = FieldFill.INSERT_UPDATE)
    private String updateBy;

    /**
     * 更新时间
     */
    @TableField(fill = FieldFill.INSERT_UPDATE)
    private LocalDateTime updateTime;
}
