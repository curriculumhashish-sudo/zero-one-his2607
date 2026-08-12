package com.zeroone.star.project.vo.j5.ons;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.math.BigDecimal;
import java.time.LocalDateTime;

/**
 * 患者耗材项目VO（映射wkf_device_dispense表）
 */
@Data
@Schema(description = "患者耗材项目VO")
public class PatientConsumableItemVO {

    @Schema(description = "记录ID")
    private String id;

    @Schema(description = "业务编号")
    private String busNo;

    @Schema(description = "设备申请ID")
    private String deviceReqId;

    @Schema(description = "状态枚举")
    private String statusEnum;

    @Schema(description = "基于的表")
    private String basedOnTable;

    @Schema(description = "基于的ID")
    private String basedOnId;

    @Schema(description = "未执行原因枚举")
    private String notPerformedReasonEnum;

    @Schema(description = "发药类别枚举")
    private String dispenseCategoryEnum;

    @Schema(description = "数量")
    private BigDecimal quantity;

    @Schema(description = "单位代码")
    private String unitCode;

    @Schema(description = "批号")
    private String lotNumber;

    @Schema(description = "设备定义ID")
    private String deviceDefId;

    @Schema(description = "设备名称")
    private String deviceName;

    @Schema(description = "患者ID")
    private String patientId;

    @Schema(description = "就诊ID")
    private String encounterId;

    @Schema(description = "执行者ID")
    private String performerId;

    @Schema(description = "执行科室ID")
    private String locationId;

    @Schema(description = "部分类型代码")
    private String partTypeCode;

    @Schema(description = "发药数量")
    private BigDecimal dispenseQuantity;

    @Schema(description = "准备者ID")
    private String preparerId;

    @Schema(description = "准备时间")
    private LocalDateTime prepareTime;

    @Schema(description = "发药时间")
    private LocalDateTime dispenseTime;

    @Schema(description = "限制时间")
    private LocalDateTime limitTime;

    @Schema(description = "计划发药时间")
    private LocalDateTime plannedDispenseTime;

    @Schema(description = "目标位置ID")
    private String desLocationId;

    @Schema(description = "推荐从业者ID")
    private String recPractitionerId;

    @Schema(description = "支持信息")
    private String supportInfo;

    @Schema(description = "用法说明")
    private String usageInstruction;

    @Schema(description = "发药频率代码")
    private String dispenseFrequencyCode;

    @Schema(description = "追踪号")
    private String traceNo;

    @Schema(description = "操作ID")
    private String procedureId;
}
