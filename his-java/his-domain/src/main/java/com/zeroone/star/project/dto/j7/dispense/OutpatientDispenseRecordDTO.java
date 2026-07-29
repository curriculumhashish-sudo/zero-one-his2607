package com.zeroone.star.project.dto.j7.dispense;

import lombok.Data;
import org.apache.fesod.sheet.annotation.ExcelIgnore;
import org.apache.fesod.sheet.annotation.ExcelProperty;
import org.apache.fesod.sheet.annotation.format.DateTimeFormat;
import org.apache.fesod.sheet.annotation.write.style.ColumnWidth;

import java.math.BigDecimal;
import java.time.LocalDateTime;


@Data
@ColumnWidth(18)
public class OutpatientDispenseRecordDTO {

    @ExcelIgnore
    private String id;

    @ExcelProperty("记录编号")
    private String recordNo;

    @ExcelProperty("操作类型")
    private String opTypeName;

    @ExcelProperty("状态")
    private String statusEnum;

    @ExcelProperty("患者姓名")
    private String patientName;

    @ExcelProperty("项目编号")
    private String clinicNo;

    @ExcelProperty("操作时间")
    @DateTimeFormat("yyyy-MM-dd HH:mm:ss")
    private LocalDateTime opTime;

    @ExcelProperty("药房位置")
    private String pharmacyName;

    @ExcelProperty("发药人")
    private String operatorAccount;

    @ExcelProperty("药品名称")
    private String drugName;

    @ExcelProperty("规格")
    private String spec;

    @ExcelProperty("数量")
    private BigDecimal quantity;

    @ExcelProperty("单位")
    private String unit;

    @ExcelProperty("批号")
    private String lotNumber;

    @ExcelProperty("用法")
    private String dosageInstruction;

    @ExcelProperty("单次剂量")
    private BigDecimal dose;

    @ExcelProperty("剂量单位")
    private String doseUnitCode;

    @ExcelProperty("频次")
    private String frequencyCode;

    @ExcelProperty("生产厂家")
    private String manufacturerText;
}
