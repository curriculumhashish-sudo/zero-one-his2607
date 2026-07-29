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
public class InpatientDispenseRecordDTO {

    @ExcelIgnore
    private String id;

    @ExcelProperty("发退药类型")
    private String dispenseTypeName;

    @ExcelProperty("发退药状态")
    private String dispenseStatusName;

    @ExcelProperty("医嘱/处方号")
    private String prescriptionNo;

    @ExcelProperty("患者姓名")
    private String patientName;

    @ExcelProperty("住院号")
    private String inpatientNo;

    @ExcelProperty("病区")
    private String wardName;

    @ExcelProperty("床号")
    private String bedNo;

    @ExcelProperty("药品名称")
    private String drugName;

    @ExcelProperty("规格")
    private String spec;

    @ExcelProperty("发退药数量")
    private BigDecimal dispenseQuantity;

    @ExcelProperty("单位")
    private String unitCode;

    @ExcelProperty("批号")
    private String lotNumber;

    @ExcelProperty("追溯码")
    private String traceNo;

    @ExcelProperty("药师")
    private String pharmacistName;

    @ExcelProperty("发退药时间")
    @DateTimeFormat("yyyy-MM-dd HH:mm:ss")
    private LocalDateTime dispenseTime;
}
