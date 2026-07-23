package com.zeroone.star.project.vo.j7.outpharm;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.math.BigDecimal;
import java.time.LocalDateTime;

/**
 * 获取患者药品项目 返回前端VO
 * 处方明细 + 药品基础信息 联合返回
 */
@Data
@Schema(description = "患者药品项目返回结果")
public class PatientDrugItemVO {

    // ===== 处方明细 medication_dispense =====
    @Schema(description = "处方明细ID")
    private String id;

    @Schema(description = "租户ID")
    private String tenantId;

    @Schema(description = "业务单号")
    private String busNo;

    @Schema(description = "处方状态(status_enum)")
    private String statusEnum;

    @Schema(description = "发药状态(dispense_enum)")
    private String dispenseEnum;

    @Schema(description = "药品定义ID")
    private String medicationId;

    @Schema(description = "患者ID")
    private String patientId;

    @Schema(description = "就诊ID")
    private String encounterId;

    @Schema(description = "医嘱ID")
    private String medReqId;

    @Schema(description = "开单数量", example = "2")
    private BigDecimal quantity;

    @Schema(description = "单位编码")
    private String unitCode;

    @Schema(description = "已发药数量", example = "1")
    private BigDecimal dispenseQuantity;

    @Schema(description = "批号", example = "B20260101")
    private String lotNumber;

    @Schema(description = "配药人ID")
    private String preparerId;

    @Schema(description = "配药时间")
    private LocalDateTime prepareTime;

    @Schema(description = "发药人ID")
    private String practitionerId;

    @Schema(description = "发药时间")
    private LocalDateTime dispenseTime;

    @Schema(description = "用法用量说明")
    private String dosageInstruction;

    @Schema(description = "给药途径编码")
    private String methodCode;

    @Schema(description = "用药频次编码")
    private String frequencyCode;

    @Schema(description = "单次剂量")
    private BigDecimal dose;

    @Schema(description = "剂量单位编码")
    private String doseUnitCode;

    @Schema(description = "汇总单号")
    private String summaryNo;

    // ===== 药品基础信息 medication_definition =====
    @Schema(description = "药品名称", example = "阿莫西林胶囊")
    private String name;

    @Schema(description = "商品名", example = "阿莫仙")
    private String merchandiseName;

    @Schema(description = "药品分类编码")
    private String categoryCode;

    @Schema(description = "规格", example = "0.25g*24粒")
    private String comprisedText;

    @Schema(description = "主要成分", example = "阿莫西林")
    private String ingredient;

    @Schema(description = "国药准字", example = "H20013001")
    private String approvalNumber;

    @Schema(description = "是否皮试(0-否 1-是)")
    private Integer skinTestFlag;

    @Schema(description = "是否注射剂(0-否 1-是)")
    private Integer injectFlag;

    @Schema(description = "生产厂家", example = "广州白云山制药")
    private String manufacturerText;

    @Schema(description = "是否抗生素(0-否 1-是)")
    private Integer antibioticFlag;

    @Schema(description = "是否处方药(0-否 1-是)")
    private Integer rxFlag;

    @Schema(description = "剂型编码")
    private String doseFormCode;

    @Schema(description = "包装单位编码")
    private String packageUnitCode;

    @Schema(description = "药品拼音码", example = "AMXL")
    private String pyStr;
}
