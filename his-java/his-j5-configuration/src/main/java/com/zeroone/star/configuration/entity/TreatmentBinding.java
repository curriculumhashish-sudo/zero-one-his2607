package com.zeroone.star.configuration.entity;

import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Data;
import lombok.EqualsAndHashCode;

import java.math.BigDecimal;

/**
 * <p>
 * 描述：诊疗绑定实体（映射cus_service_device_template表）
 * </p>
 *
 * @author 扶舒
 * @version 1.0.0
 */
@Data
@EqualsAndHashCode(callSuper = true)
@TableName("cus_service_device_template")
public class TreatmentBinding extends BaseEntity {

    /**
     * 范围编码
     */
    private String rangeCode;

    /**
     * 类型编码
     */
    private String typeCode;

    /**
     * 项目表（如：healthcare_service）
     */
    private String itemTable;

    /**
     * 项目ID
     */
    private String itemId;

    /**
     * 设备行为表（如：device_definition）
     */
    private String devActTable;

    /**
     * 设备行为ID
     */
    private String devActId;

    /**
     * 数量
     */
    private BigDecimal quantity;

    /**
     * 单位编码
     */
    private String unitCode;

    /**
     * 状态枚举
     */
    private String statusEnum;
}
