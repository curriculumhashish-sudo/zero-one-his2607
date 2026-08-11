package com.zeroone.star.pharmacy.entity;

import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Data;
import lombok.Getter;
import lombok.Setter;

import java.io.Serializable;
import java.math.BigDecimal;
import java.time.LocalDate;

/**
 * <p>
 * 描述：入库单明细实体类，明细以“同表多行”方式存储于 {@code wkf_supply_request}，
 * 与主表通过 {@code bus_no} 关联（与盘点模块一致），药品名称等经 {@code med_medication_definition} 关联回显
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
@TableName("wkf_supply_request")
public class InStockOrderDetail implements Serializable {

    @TableId(value = "id", type = IdType.ASSIGN_ID)
    private String id;

    /** 关联主表业务单号，对应 wkf_supply_request.bus_no */
    private String pharmacyId;

    private Integer seqNo;
    private String itemId;
    private String drugName;
    private String spec;
    private String manufacturer;
    private Integer quantity;
    private BigDecimal unitPrice;
    private BigDecimal totalAmount;
    private String batchNo;
    private String traceNo;
    private LocalDate expireDate;
    private String itemRemark;
}