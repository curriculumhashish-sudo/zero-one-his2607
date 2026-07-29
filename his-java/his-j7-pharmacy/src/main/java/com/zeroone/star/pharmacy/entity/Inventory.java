package com.zeroone.star.pharmacy.entity;

import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableLogic;
import com.baomidou.mybatisplus.annotation.TableName;
import com.baomidou.mybatisplus.annotation.Version;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

import java.io.Serial;
import java.io.Serializable;
import java.math.BigDecimal;
import java.time.LocalDateTime;

/**
 * <p>
 * 描述：库存项实体类，对应数据库表 {@code wkf_inventory_item}
 * </p>
 * <p>
 * 功能说明：该表为库存项聚合根，记录药品/器材在库房中的库存信息。
 * 包含物品名称、规格、库存数量、批号、有效期、供应商、货位等核心字段。
 * 在发药业务中，发药时扣减库存数量（{@code quantity}）；在退药业务中，
 * 确认退药后回补库存数量。库存状态（{@code inventory_status_enum}）
 * 为 ACTIVE 表示有效库存，INACTIVE 表示失效库存。
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 01star
 * @version 1.0.0
 */
@Getter
@Setter
@ToString
@TableName("wkf_inventory_item")
public class Inventory implements Serializable {

    @Serial
    private static final long serialVersionUID = 1L;

    /**
     * 主键ID
     * 对应表字段：id，采用雪花算法自动生成
     */
    @TableId(value = "id", type = IdType.ASSIGN_ID)
    private String id;

    /**
     * 租户标识
     * 对应表字段：tenant_id
     */
    private String tenantId;

    /**
     * 创建时间
     * 对应表字段：created_at
     */
    private LocalDateTime createdAt;

    /**
     * 最后更新时间
     * 对应表字段：updated_at
     */
    private LocalDateTime updatedAt;

    /**
     * 创建人
     * 对应表字段：created_by
     */
    private String createdBy;

    /**
     * 最后更新人
     * 对应表字段：updated_by
     */
    private String updatedBy;

    /**
     * 软删标记
     * 对应表字段：is_deleted
     */
    @TableLogic
    private Boolean isDeleted;

    /**
     * 删除时间
     * 对应表字段：deleted_at
     */
    private LocalDateTime deletedAt;

    /**
     * 删除人
     * 对应表字段：deleted_by
     */
    private String deletedBy;

    /**
     * 乐观锁版本号
     * 对应表字段：version
     */
    @Version
    private Integer version;

    /**
     * 库存项分类编码
     * 对应表字段：category_code
     */
    private String categoryCode;

    /**
     * 物品定义表名
     * 对应表字段：item_table，关联物品定义所在的表
     */
    private String itemTable;

    /**
     * 物品定义标识
     * 对应表字段：item_id，关联物品定义主键
     */
    private String itemId;

    /**
     * 物品名称（冗余）
     * 对应表字段：name
     */
    private String name;

    /**
     * 拼音码
     * 对应表字段：py_str，用于检索
     */
    private String pyStr;

    /**
     * 五笔码
     * 对应表字段：wb_str，用于检索
     */
    private String wbStr;

    /**
     * 库存状态枚举
     * 对应表字段：inventory_status_enum
     * ACTIVE-有效 INACTIVE-失效，与 PublicationStatus 编码对齐
     */
    private String inventoryStatusEnum;

    /**
     * 供应商标识
     * 对应表字段：supplier_id
     */
    private String supplierId;

    /**
     * 描述文本
     * 对应表字段：description_text
     */
    private String descriptionText;

    /**
     * 计量单位编码
     * 对应表字段：unit_code
     */
    private String unitCode;

    /**
     * 当前库存数量
     * 对应表字段：quantity
     */
    private BigDecimal quantity;

    /**
     * 特征扩展 JSON
     * 对应表字段：characteristic_json
     */
    private String characteristicJson;

    /**
     * 产品批号
     * 对应表字段：lot_number
     */
    private String lotNumber;

    /**
     * 生产日期
     * 对应表字段：production_date
     */
    private LocalDateTime productionDate;

    /**
     * 效期
     * 对应表字段：expiration_date
     */
    private LocalDateTime expirationDate;

    /**
     * 有效期月数
     * 对应表字段：validity_mon
     */
    private Integer validityMon;

    /**
     * 货位标识
     * 对应表字段：location_store_id
     */
    private String locationStoreId;

    /**
     * 库房标识
     * 对应表字段：location_id
     */
    private String locationId;

    /**
     * 追溯码
     * 对应表字段：trace_no
     */
    private String traceNo;

    /**
     * 追溯码包装层级
     * 对应表字段：packaging_levels
     */
    private Integer packagingLevels;

    /**
     * 判断是否低于库存警戒值
     * @param minQuantity 最低警戒数量
     * @return true-低于警戒值，false-正常
     */
    public boolean isLowStock(BigDecimal minQuantity) {
        return quantity != null && quantity.compareTo(minQuantity) < 0;
    }

    /**
     * 判断库存是否已过期
     * @return true-已过期，false-未过期
     */
    public boolean isExpired() {
        return expirationDate != null && expirationDate.isBefore(LocalDateTime.now());
    }

    /**
     * 判断库存是否即将过期
     * @param daysThreshold 过期阈值天数
     * @return true-即将过期，false-正常
     */
    public boolean isNearExpiry(int daysThreshold) {
        if (expirationDate == null) {
            return false;
        }
        return expirationDate.isBefore(LocalDateTime.now().plusDays(daysThreshold));
    }

    /**
     * 减少库存数量
     * @param amount 减少的数量
     */
    public void decreaseQuantity(BigDecimal amount) {
        if (quantity == null) {
            quantity = BigDecimal.ZERO;
        }
        quantity = quantity.subtract(amount);
    }

    /**
     * 增加库存数量
     * @param amount 增加的数量
     */
    public void increaseQuantity(BigDecimal amount) {
        if (quantity == null) {
            quantity = BigDecimal.ZERO;
        }
        quantity = quantity.add(amount);
    }
}