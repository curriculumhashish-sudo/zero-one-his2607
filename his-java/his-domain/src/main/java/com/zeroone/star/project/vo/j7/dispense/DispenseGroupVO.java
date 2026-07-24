package com.zeroone.star.project.vo.j7.dispense;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.util.List;

@Data
@Schema(description = "门诊发退药记录（按处方分组）")
public class DispenseGroupVO {

    @Schema(description = "处方号", example = "CP2026-07-010003")
    private String prescriptionNo;

    @Schema(description = "类型", example = "发药")
    private String recordTypeName;

    @Schema(description = "患者姓名", example = "马化腾")
    private String patientName;

    @Schema(description = "门诊号", example = "EN2026-07-010001")
    private String clinicNo;

    @Schema(description = "开单科室", example = "门诊内科")
    private String deptName;

    @Schema(description = "发药时间", example = "2026-07-01 19:25:06")
    private String dispenseTime;

    @Schema(description = "药房", example = "门诊中药房")
    private String pharmacyName;

    @Schema(description = "发药人", example = "admin")
    private String dispenserName;

    @Schema(description = "药品明细列表")
    private List<DrugItemVO> drugItems;
}