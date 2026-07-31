package com.zeroone.star.configuration.entity;

import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Data;
import lombok.EqualsAndHashCode;

import java.math.BigDecimal;

/**
 * <p>
 * 描述：取药科室库存DO
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author zeroone
 * @version 1.0.0
 */
@Data
@EqualsAndHashCode(callSuper = true)
@TableName("dept_stock")
public class DeptStockDO extends BaseEntity {

    /**
     * 科室ID
     */
    private String deptId;

    /**
     * 科室名称
     */
    private String deptName;

    /**
     * 物品ID
     */
    private String itemId;

    /**
     * 物品名称
     */
    private String itemName;

    /**
     * 物品编码
     */
    private String itemCode;

    /**
     * 物品类型：DRUG-药品,CONSUMABLE-耗材
     */
    private String itemType;

    /**
     * 规格
     */
    private String specification;

    /**
     * 单位
     */
    private String unit;

    /**
     * 库存数量
     */
    private BigDecimal stockQuantity;

    /**
     * 安全库存
     */
    private BigDecimal safetyStock;

    /**
     * 是否启用：0-禁用，1-启用
     */
    private Integer isEnabled;

    /**
     * 备注
     */
    private String remark;
}
