package com.zeroone.star.pharmacy.entity;

import com.baomidou.mybatisplus.annotation.*;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

import java.io.Serial;
import java.io.Serializable;
import java.math.BigDecimal;

/**
 * <p>
 *     描述：盘点单明细实体类，对应数据库表 {@code stock_take_order_detail}
 * </p>
 * <p>
 *     功能说明：存储盘点单的每一条药品明细，包括账面库存、实盘数量、盈亏计算等。
 *     通过 {@code stock_take_id} 与盘点单主表关联，支持分页查询、批量更新等操作。
 *     服务层根据盈亏数量自动计算盈亏金额。
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 01star
 * @version 1.0.0
 */
@Getter
@Setter
@ToString
@TableName("stock_take_order_detail")
public class StockTakeOrderDetail implements Serializable {

    @Serial
    private static final long serialVersionUID = 1L;

    /**
     * 主键ID
     */
    @TableId(value = "id", type = IdType.AUTO)
    private Long id;

    /**
     * 盘点单主表ID (关联 stock_take_order.id)
     */
    @TableField("stock_take_id")
    private Long stockTakeId;

    /**
     * 盘点单号 (业务唯一标识)
     */
    @TableField("stock_take_no")
    private String stockTakeNo;

    /**
     * 明细序号 (行号)
     */
    @TableField("seq_no")
    private Integer seqNo;

    /**
     * 药品名称
     */
    @TableField("drug_name")
    private String drugName;

    /**
     * 药品规格
     */
    @TableField("spec")
    private String spec;

    /**
     * 生产厂家
     */
    @TableField("manufacturer")
    private String manufacturer;

    /**
     * 药品批号
     */
    @TableField("batch_no")
    private String batchNo;

    /**
     * 追溯码 (电子监管码)
     */
    @TableField("trace_code")
    private String traceCode;

    /**
     * 当前库存量 (账面库存)
     */
    @TableField("stock_qty")
    private Integer stockQty;

    /**
     * 实盘数量 (实地盘点数量)
     */
    @TableField("actual_qty")
    private Integer actualQty;

    /**
     * 盈亏数量 (actual_qty - stock_qty)
     */
    @TableField("diff_qty")
    private Integer diffQty;

    /**
     * 药品单价 (成本价)
     */
    @TableField("unit_price")
    private BigDecimal unitPrice;

    /**
     * 实盘金额 (actual_qty * unit_price)
     */
    @TableField("actual_amount")
    private BigDecimal actualAmount;

    /**
     * 盈亏金额 (diff_qty * unit_price)
     */
    @TableField("diff_amount")
    private BigDecimal diffAmount;

    /**
     * 明细备注
     */
    @TableField("item_remark")
    private String itemRemark;
}