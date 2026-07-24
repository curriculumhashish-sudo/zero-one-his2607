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
 * 描述：发药明细表实体类，对应数据库表 {@code dispensing_item}
 * </p>
 * <p>
 * 功能说明：记录发药单据中的药品明细信息，包含药品编码、名称、规格、
 * 单价、数量、金额等。与发药主表 dispensing_header 为多对一关系，
 * 通过 dispensingId 关联。在查看发药药品项目时通过该表获取药品详情。
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 01star
 * @version 1.0.0
 */
@Getter
@Setter
@TableName("dispensing_item")
public class DispensingItem implements Serializable {
    private static final long serialVersionUID = 1L;

    /**
     * 主键ID
     * 对应表字段：id，采用雪花算法自动生成
     */
    @TableId(value = "id", type = IdType.ASSIGN_ID)
    private String id;

    /**
     * 发药单据ID
     * 对应表字段：dispensing_id，关联 dispensing_header 表
     */
    private String dispensingId;

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
     * 对应表字段：spec，例如 0.25g*12片/盒
     */
    private String spec;

    /**
     * 单价
     * 对应表字段：unit_price
     */
    private BigDecimal unitPrice;

    /**
     * 数量
     * 对应表字段：quantity
     */
    private Integer quantity;

    /**
     * 单位
     * 对应表字段：unit，例如 盒、瓶、支
     */
    private String unit;

    /**
     * 金额
     * 对应表字段：amount，计算公式：单价 × 数量
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
