package com.zeroone.star.statdata.entity;

import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Data;

import java.math.BigDecimal;
import java.time.LocalDateTime;
/**
 * <p>描述: 库存物资主表</p>
 * <p>版权: &copy;01星球</p>
 * <p>地址: 01星球总部</p>
 *
 * @author highxin
 * @version 1.0.0
 */
@Data
@TableName("wkf_inventory_item")
public class WkfInventoryItem {

    private String id;
    private String tenantId;
    private LocalDateTime createdAt;
    private LocalDateTime updatedAt;
    private String createdBy;
    private String updatedBy;
    private Integer isDeleted;
    private LocalDateTime deletedAt;
    private String deletedBy;
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