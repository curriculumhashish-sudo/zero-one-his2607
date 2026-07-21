package com.zeroone.star.pharmacy.entity;

import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Getter;
import lombok.Setter;

import java.io.Serializable;
import java.math.BigDecimal;

/**
 * <p>
 * 描述：退药申请明细表实体类，对应数据库表 {@code return_item}
 * </p>
 * <p>
 * 功能说明：记录退药申请单中的药品明细信息，包含药品编码、名称、规格、
 * 退药数量、金额等。与退药主表 return_header 为多对一关系，
 * 通过 returnId 关联。在查看退药明细和确认退药时使用。
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 01star
 * @version 1.0.0
 */
@Getter
@Setter
@TableName("return_item")
public class ReturnItem implements Serializable {
    private static final long serialVersionUID = 1L;

    /**
     * 主键ID
     * 对应表字段：id，采用雪花算法自动生成
     */
    @TableId(value = "id", type = IdType.ASSIGN_ID)
    private String id;

    /**
     * 退药申请单ID
     * 对应表字段：return_id，关联 return_header 表
     */
    private String returnId;

    /**
     * 药品编码
     * 对应表字段：drug_code
     */
    private String drugCode;

    /**
     * 药品名称
     * 对应表字段：drug_name
     */
    private String drugName;

    /**
     * 规格
     * 对应表字段：spec
     */
    private String spec;

    /**
     * 单价
     * 对应表字段：unit_price
     */
    private BigDecimal unitPrice;

    /**
     * 退药数量
     * 对应表字段：quantity
     */
    private Integer quantity;

    /**
     * 单位
     * 对应表字段：unit
     */
    private String unit;

    /**
     * 金额
     * 对应表字段：amount
     */
    private BigDecimal amount;

    /**
     * 生产批号
     * 对应表字段：batch_no
     */
    private String batchNo;

    /**
     * 有效期
     * 对应表字段：expire_date
     */
    private String expireDate;
}
