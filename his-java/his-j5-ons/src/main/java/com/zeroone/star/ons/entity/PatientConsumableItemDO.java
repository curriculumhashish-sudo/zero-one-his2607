package com.zeroone.star.ons.entity;

import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Data;
import lombok.EqualsAndHashCode;

import java.math.BigDecimal;

/**
 * <p>
 * 描述：患者耗材项目DO
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author zeroone
 * @version 1.0.0
 */
@Data
@EqualsAndHashCode(callSuper = true)
@TableName("patient_consumable_item")
public class PatientConsumableItemDO extends BaseEntity {

    /**
     * 业务编号
     */
    private String busNo;

    /**
     * 患者ID
     */
    private String patientId;

    /**
     * 就诊ID
     */
    private String encounterId;

    /**
     * 项目ID
     */
    private String itemId;

    /**
     * 项目名称
     */
    private String itemName;

    /**
     * 项目编码
     */
    private String itemCode;

    /**
     * 耗材类型
     */
    private String consumableType;

    /**
     * 数量
     */
    private BigDecimal quantity;

    /**
     * 单位代码
     */
    private String unitCode;

    /**
     * 单位名称
     */
    private String unitName;

    /**
     * 单价
     */
    private BigDecimal unitPrice;

    /**
     * 总价
     */
    private BigDecimal totalPrice;

    /**
     * 库存数量
     */
    private BigDecimal stockQuantity;

    /**
     * 库存单位
     */
    private String stockUnit;

    /**
     * 生产厂家
     */
    private String manufacturer;

    /**
     * 批号
     */
    private String batchNo;

    /**
     * 有效期
     */
    private String expiryDate;

    /**
     * 执行科室ID
     */
    private String execDeptId;

    /**
     * 执行科室名称
     */
    private String execDeptName;

    /**
     * 状态
     */
    private String status;

    /**
     * 执行状态
     */
    private Integer executionStatus;

    /**
     * 备注
     */
    private String remark;
}
