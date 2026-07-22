package com.zeroone.star.project.vo.j7.dispense;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.math.BigDecimal;
import java.time.LocalDateTime;

@Data
@Schema(description = "住院发退药记录")
public class InpatientDispenseRecordVO {

    @Schema(description = "发退药记录ID", example = "DISP-001")
    private String id;

    @Schema(description = "发退药类型", example = "发药")
    private String dispenseTypeName;

    @Schema(description = "发退药状态", example = "已完成")
    private String dispenseStatusName;

    @Schema(description = "医嘱或处方号", example = "RX-20260701-001")
    private String prescriptionNo;

    @Schema(description = "患者姓名", example = "张三")
    private String patientName;

    @Schema(description = "住院号", example = "IP-20260701-001")
    private String inpatientNo;

    @Schema(description = "病区", example = "内科一病区")
    private String wardName;

    @Schema(description = "床号", example = "08床")
    private String bedNo;

    @Schema(description = "药品名称", example = "阿托伐他汀钙片")
    private String drugName;

    @Schema(description = "药品规格", example = "20mg*7片")
    private String spec;

    @Schema(description = "发退药数量", example = "2")
    private BigDecimal dispenseQuantity;

    @Schema(description = "单位编码", example = "盒")
    private String unitCode;

    @Schema(description = "药品批号", example = "LOT-20260701")
    private String lotNumber;

    @Schema(description = "药品追溯码", example = "TRACE-001")
    private String traceNo;

    @Schema(description = "发药药师", example = "李药师")
    private String pharmacistName;

    @Schema(description = "发退药时间", example = "2026-07-01 19:25:06")
    private LocalDateTime dispenseTime;
}
