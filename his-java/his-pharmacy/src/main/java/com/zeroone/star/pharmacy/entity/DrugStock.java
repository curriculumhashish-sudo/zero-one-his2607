package com.zeroone.star.pharmacy.entity;

import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Getter;
import lombok.Setter;

import java.io.Serializable;
import java.math.BigDecimal;
import java.time.LocalDateTime;

/**
 * <p>
 * 描述：药品库存表实体类，对应数据库表 {@code drug_stock}
 * </p>
 * <p>
 * 功能说明：记录药品的库存信息，包含药品编码、名称、规格、库存数量、
 * 批号、有效期、供应商等。在住院退药确认流程中，
 * 确认退药后会回补该药品的库存数量（stock_quantity 增加退药数量）。
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 01star
 * @version 1.0.0
 */
@Getter
@Setter
@TableName("drug_stock")
public class DrugStock implements Serializable {
    private static final long serialVersionUID = 1L;

    /**
     * 主键ID
     * 对应表字段：id，采用雪花算法自动生成
     */
    @TableId(value = "id", type = IdType.ASSIGN_ID)
    private String id;

    /**
     * 药品编码
     * 对应表字段：drug_code，药品唯一标识
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
     * 库存数量
     * 对应表字段：stock_quantity，当前库存量
     */
    private Integer stockQuantity;

    /**
     * 单位
     * 对应表字段：unit
     */
    private String unit;

    /**
     * 单价
     * 对应表字段：unit_price
     */
    private BigDecimal unitPrice;

    /**
     * 生产批号
     * 对应表字段：batch_no
     */
    private String batchNo;

    /**
     * 有效期
     * 对应表字段：expire_date
     */
    private LocalDateTime expireDate;

    /**
     * 供应商
     * 对应表字段：supplier
     */
    private String supplier;

    /**
     * 更新时间
     * 对应表字段：update_time，库存变动时更新
     */
    private LocalDateTime updateTime;
}
