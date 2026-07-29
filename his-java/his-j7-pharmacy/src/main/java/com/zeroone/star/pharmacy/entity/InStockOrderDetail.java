package com.zeroone.star.pharmacy.entity;

import com.baomidou.mybatisplus.annotation.*;
import lombok.Data;
import java.math.BigDecimal;
import java.time.LocalDate;

@Data
@TableName("pharmacy_in_item")
public class InStockOrderDetail {
    @TableId(type = IdType.AUTO)
    private Long id;
    private Long pharmacyId;
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