package com.zeroone.star.pharmacy.entity;

import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Data;

import java.io.Serializable;
import java.math.BigDecimal;
import java.time.LocalDate;

/**
 * <p>
 * 描述：入库单明细实体类，对应数据库表 {@code pharmacy_in_item}
 * </p>
 * <p>
 * 功能说明：存储每条入库药品的详细信息，通过 {@code pharmacy_id} 与主表关联。
 * 行合计金额由服务端计算后写入。
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 01star
 * @version 1.0.0
 */
@Data
@TableName("pharmacy_in_item")
public class InStockOrderDetail implements Serializable {

    @TableId(type = IdType.AUTO)
    private Long id;

    /** 关联主表 ID，对应 wkf_supply_request.id */
    private String pharmacyId;

    private Integer seqNo;
    private String drugName;
    private String spec;
    private String manufacturer;
    private Integer quantity;
    private BigDecimal unitPrice;
    private BigDecimal totalAmount;
    private String batchNo;
    private LocalDate expireDate;
    private String itemRemark;
}