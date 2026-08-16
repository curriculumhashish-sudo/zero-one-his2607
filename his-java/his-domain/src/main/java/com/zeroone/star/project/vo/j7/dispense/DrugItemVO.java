package com.zeroone.star.project.vo.j7.dispense;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.math.BigDecimal;

@Data
@Schema(description = "处方药品明细")
public class DrugItemVO {

    @Schema(description = "药品名称", example = "阿托伐他汀钙片")
    private String drugName;

    @Schema(description = "规格", example = "20mg*7片")
    private String spec;

    @Schema(description = "数量", example = "2")
    private BigDecimal quantity;

    @Schema(description = "单位", example = "盒")
    private String unit;

    @Schema(description = "批号", example = "B20260701001")
    private String lotNumber;

    @Schema(description = "用法", example = "口服")
    private String dosageInstruction;

    @Schema(description = "单次剂量", example = "10")
    private BigDecimal dose;

    @Schema(description = "剂量单位", example = "mg")
    private String doseUnitCode;

    @Schema(description = "频次", example = "每日三次")
    private String frequencyCode;

    @Schema(description = "生产厂家", example = "辉瑞制药")
    private String manufacturerText;
}