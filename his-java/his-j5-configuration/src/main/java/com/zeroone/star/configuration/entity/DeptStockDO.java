package com.zeroone.star.configuration.entity;

import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Data;
import lombok.EqualsAndHashCode;

import java.math.BigDecimal;
import java.time.LocalDateTime;

/**
 * <p>
 * 描述：取药科室库存DO（映射wkf_inventory_item表）
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author zeroone
 * @version 1.0.0
 */
@Data
@EqualsAndHashCode(callSuper = true)
@TableName("wkf_inventory_item")
public class DeptStockDO extends BaseEntity {

    /**
     * 分类编码
     */
    private String categoryCode;

    /**
     * 物品表（如：drug, consumable）
     */
    private String itemTable;

    /**
     * 物品ID
     */
    private String itemId;

    /**
     * 物品名称
     */
    private String name;

    /**
     * 拼音码
     */
    private String pyStr;

    /**
     * 五笔码
     */
    private String wbStr;

    /**
     * 库存状态枚举
     */
    private String inventoryStatusEnum;

    /**
     * 供应商ID
     */
    private String supplierId;

    /**
     * 描述文本
     */
    private String descriptionText;

    /**
     * 单位编码
     */
    private String unitCode;

    /**
     * 数量
     */
    private BigDecimal quantity;

    /**
     * 特性JSON
     */
    private String characteristicJson;

    /**
     * 批号
     */
    private String lotNumber;

    /**
     * 生产日期
     */
    private LocalDateTime productionDate;

    /**
     * 有效期
     */
    private LocalDateTime expirationDate;

    /**
     * 有效期（月）
     */
    private Integer validityMon;

    /**
     * 库存地点ID
     */
    private String locationStoreId;

    /**
     * 执行科室ID
     */
    private String locationId;

    /**
     * 追踪号
     */
    private String traceNo;

    /**
     * 包装层级
     */
    private Integer packagingLevels;
}
