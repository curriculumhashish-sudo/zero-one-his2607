package com.zeroone.star.ons.entity;

import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Data;
import lombok.EqualsAndHashCode;

import java.math.BigDecimal;
import java.time.LocalDateTime;

/**
 * <p>
 * 描述：患者耗材项目DO（映射 wkf_device_dispense 表）
 * </p>
 * <p>版权：&copy;01星球</p>
 * @author zeroone
 * @version 1.0.0
 */
@Data
@EqualsAndHashCode(callSuper = true)
@TableName("wkf_device_dispense")
public class PatientConsumableItemDO extends BaseEntity {

    /**
     * 业务编号
     */
    private String busNo;

    /**
     * 设备申请ID
     */
    private String deviceReqId;

    /**
     * 状态枚举
     */
    private String statusEnum;

    /**
     * 基于的表
     */
    private String basedOnTable;

    /**
     * 基于的ID
     */
    private String basedOnId;

    /**
     * 未执行原因枚举
     */
    private String notPerformedReasonEnum;

    /**
     * 发药类别枚举
     */
    private String dispenseCategoryEnum;

    /**
     * 数量
     */
    private BigDecimal quantity;

    /**
     * 单位代码
     */
    private String unitCode;

    /**
     * 批号
     */
    private String lotNumber;

    /**
     * 设备定义ID
     */
    private String deviceDefId;

    /**
     * 患者ID
     */
    private String patientId;

    /**
     * 就诊ID
     */
    private String encounterId;

    /**
     * 执行者ID
     */
    private String performerId;

    /**
     * 执行科室ID
     */
    private String locationId;

    /**
     * 部分类型代码
     */
    private String partTypeCode;

    /**
     * 发药数量
     */
    private BigDecimal dispenseQuantity;

    /**
     * 准备者ID
     */
    private String preparerId;

    /**
     * 准备时间
     */
    private LocalDateTime prepareTime;

    /**
     * 发药时间
     */
    private LocalDateTime dispenseTime;

    /**
     * 限制时间
     */
    private LocalDateTime limitTime;

    /**
     * 计划发药时间
     */
    private LocalDateTime plannedDispenseTime;

    /**
     * 目标位置ID
     */
    private String desLocationId;

    /**
     * 推荐从业者ID
     */
    private String recPractitionerId;

    /**
     * 支持信息
     */
    private String supportInfo;

    /**
     * 用法说明
     */
    private String usageInstruction;

    /**
     * 发药频率代码
     */
    private String dispenseFrequencyCode;

    /**
     * 追踪号
     */
    private String traceNo;

    /**
     * 操作ID
     */
    private String procedureId;
}
