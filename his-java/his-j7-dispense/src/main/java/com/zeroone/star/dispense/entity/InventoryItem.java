package com.zeroone.star.dispense.entity;

import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableLogic;
import com.baomidou.mybatisplus.annotation.TableName;
import com.baomidou.mybatisplus.annotation.Version;
import lombok.Getter;
import lombok.Setter;

import java.io.Serializable;
import java.math.BigDecimal;
import java.time.LocalDateTime;

@Getter
@Setter
@TableName("wkf_inventory_item")
public class InventoryItem implements Serializable {
    private static final long serialVersionUID = 1L;
    @TableId
    private String id;

    private String tenantId;

    private LocalDateTime createdAt;

    private LocalDateTime updatedAt;

    private String createdBy;

    private String updatedBy;

    @TableLogic
    private Boolean isDeleted;

    private LocalDateTime deletedAt;

    private String deletedBy;

    @Version
    private Integer version;

    private String categoryCode;

    private String itemTable;

    private String itemId;

    private String name;

    private String pyStr;

    private String wbStr;

    private String inventoryStatusEnum;

    private String supplierId;

    private String descriptionText;

    private String unitCode;

    private BigDecimal quantity;

    private String characteristicJson;

    private String lotNumber;

    private LocalDateTime productionDate;

    private LocalDateTime expirationDate;

    private Integer validityMon;

    private String locationStoreId;

    private String locationId;

    private String traceNo;

    private Integer packagingLevels;
}