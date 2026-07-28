package com.zeroone.star.project.dto.j7.dispense;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.math.BigDecimal;

@Data
@Schema(description = "药品明细")
public class DrugItem {

    @Schema(description = "处方明细ID", example = "329891909520592897", requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;

    @Schema(description = "药品ID", example = "323999010000000012", requiredMode = Schema.RequiredMode.REQUIRED)
    private String medicationId;

    @Schema(description = "药品编码", example = "MD0031")
    private String drugCode;

    @Schema(description = "药品名称", example = "当归", requiredMode = Schema.RequiredMode.REQUIRED)
    private String drugName;

    @Schema(description = "规格", example = "饮片")
    private String spec;

    @Schema(description = "拼音码", example = "DG")
    private String pyStr;

    @Schema(description = "剂型", example = "片剂")
    private String doseForm;

    @Schema(description = "生产厂家", example = "广州白云山制药")
    private String manufacturer;

    @Schema(description = "国药准字", example = "Z62020001")
    private String approvalNumber;

    @Schema(description = "应发数量", example = "30.00", requiredMode = Schema.RequiredMode.REQUIRED)
    private BigDecimal quantity;

    @Schema(description = "实发数量", example = "30.00")
    private BigDecimal dispenseQuantity;

    @Schema(description = "单位", example = "盒")
    private String unit;

    @Schema(description = "批号", example = "001-011")
    private String lotNumber;

    @Schema(description = "用法用量", example = "口服，每日三次")
    private String dosageInstruction;

    @Schema(description = "状态", example = "PENDING")
    private String statusEnum;

    @Schema(description = "追溯码", example = "TRACE-001")
    private String traceNo;

    @Schema(description = "是否可勾选", example = "true")
    private Boolean selectable;

    @Schema(description = "是否启用", example = "true")
    private Boolean activeFlag;
}