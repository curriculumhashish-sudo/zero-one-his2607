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
 * 描述：库存项实体类，对应数据库表 {@code wkf_inventory_item}
 * </p>
 * <p>
 * 功能说明：记录库存物品信息，包含物品定义表名和标识、名称、库存状态、
 * 供应商、数量、批号、生产日期、效期、货位、库房位置等。
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 01star
 * @version 1.0.0
 */
@Getter
@Setter
@TableName("wkf_inventory_item")
public class WkfInventoryItem implements Serializable {

    private static final long serialVersionUID = 1L;

    @TableId(value = "id", type = IdType.ASSIGN_ID)
    private String id;

    /** 租户标识 */
    private String tenantId;

    /** 创建时间 */
    private LocalDateTime createdAt;

    /** 最后更新时间 */
    private LocalDateTime updatedAt;

    /** 创建人 */
    private String createdBy;

    /** 最后更新人 */
    private String updatedBy;

    /** 软删标记 */
    private Integer isDeleted;

    /** 删除时间 */
    private LocalDateTime deletedAt;

    /** 删除人 */
    private String deletedBy;

    /** 乐观锁版本号 */
    private Integer version;

    /** 库存项聚合根 */
    private String categoryCode;

    /** 物品定义表名 */
    private String itemTable;

    /** 物品定义标识 */
    private String itemId;

    /** 物品名称（冗余） */
    private String name;

    /** 拼音码 */
    private String pyStr;

    /** 五笔码 */
    private String wbStr;

    /** 库存状态（ACTIVE/INACTIVE） */
    private String inventoryStatusEnum;

    /** 供应商标识 */
    private String supplierId;

    /** 描述文本 */
    private String descriptionText;

    /** 计量单位编码 */
    private String unitCode;

    /** 当前库存数量 */
    private BigDecimal quantity;

    /** 特征扩展 JSON */
    private String characteristicJson;

    /** 产品批号 */
    private String lotNumber;

    /** 生产日期 */
    private LocalDateTime productionDate;

    /** 效期 */
    private LocalDateTime expirationDate;

    /** 有效期月数 */
    private Integer validityMon;

    /** 货位标识 */
    private String locationStoreId;

    /** 库房标识 */
    private String locationId;

    /** 追溯码 */
    private String traceNo;

    /** 追溯码包装层级 */
    private Integer packagingLevels;
}
