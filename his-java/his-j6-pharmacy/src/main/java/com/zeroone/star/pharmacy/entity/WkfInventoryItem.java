package com.zeroone.star.pharmacy.entity;

import com.baomidou.mybatisplus.annotation.TableField;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Getter;
import lombok.Setter;

import java.io.Serializable;
import java.math.BigDecimal;
import java.time.LocalDateTime;

@Getter
@Setter
@TableName("wkf_inventory_item")
public class WkfInventoryItem implements Serializable {
    private static final long serialVersionUID = 1L;

    @TableId("id")
    private String id;

    @TableField("item_table")
    private String itemTable;

    @TableField("item_id")
    private String itemId;

    @TableField("unit_code")
    private String unitCode;

    private BigDecimal quantity;

    @TableField("lot_number")
    private String lotNumber;

    @TableField("location_id")
    private String locationId;

    @TableField("updated_at")
    private LocalDateTime updatedAt;

    @TableField("updated_by")
    private String updatedBy;

    @TableField("is_deleted")
    private Integer isDeleted;
}
