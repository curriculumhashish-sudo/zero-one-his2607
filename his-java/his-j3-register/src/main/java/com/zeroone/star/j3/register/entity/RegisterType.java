package com.zeroone.star.j3.register.entity;

import com.baomidou.mybatisplus.annotation.FieldFill;
import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableField;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

import java.io.Serializable;
import java.math.BigDecimal;
import java.time.LocalDateTime;

/**
 * <p>
 * 描述：挂号类型实体（映射 register_type 表）
 * </p>
 * <p>对应挂号类型：普通号、专家号、特需号等</p>
 *
 * @author j3小组
 * @version 1.0.0
 */
@Getter
@Setter
@ToString
@TableName("register_type")
public class RegisterType implements Serializable {

    private static final long serialVersionUID = 1L;

    /**
     * 挂号类型唯一标识
     */
    @TableId(value = "id", type = IdType.AUTO)
    private Long id;

    /**
     * 挂号类型名称
     */
    private String name;

    /**
     * 医保编码
     */
    private String mdtrtCertNo;

    /**
     * 业务编码
     */
    private String businessCode;

    /**
     * 金额
     */
    private BigDecimal amount;

    /**
     * 状态：1 启用 0 禁用
     */
    private Integer status;

    /**
     * 创建时间
     */
    @TableField(fill = FieldFill.INSERT)
    private LocalDateTime createTime;

    /**
     * 更新时间
     */
    @TableField(fill = FieldFill.INSERT_UPDATE)
    private LocalDateTime updateTime;

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
}
