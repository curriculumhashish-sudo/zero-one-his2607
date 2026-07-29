package com.zeroone.star.pharmacy.entity;

import com.baomidou.mybatisplus.annotation.*;
import lombok.Data;
import java.math.BigDecimal;
import java.time.LocalDate;
import java.time.LocalDateTime;

@Data
@TableName("pharmacy_in_order")
public class InStockOrder {
    @TableId(type = IdType.AUTO)
    private Long id;
    private String orderNo;
    private Long supplierId;
    private LocalDate inDate;
    private Long warehouseId;
    private BigDecimal totalAmount;
    /** 0-草稿 1-待审批 2-已审批 3-入库完成 */
    private Integer status;
    private String remark;
    @TableField(fill = FieldFill.INSERT)
    private LocalDateTime createTime;
    @TableField(fill = FieldFill.INSERT_UPDATE)
    private LocalDateTime updateTime;
}