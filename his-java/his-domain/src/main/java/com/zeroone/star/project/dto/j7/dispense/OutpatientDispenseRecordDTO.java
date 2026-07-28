package com.zeroone.star.project.dto.j7.dispense;

import lombok.Data;
import org.apache.fesod.sheet.annotation.ExcelIgnore;
import org.apache.fesod.sheet.annotation.ExcelProperty;
import org.apache.fesod.sheet.annotation.format.DateTimeFormat;
import org.apache.fesod.sheet.annotation.write.style.ColumnWidth;

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

    @ExcelProperty("操作人")
    private String patientName;

    @ExcelProperty("项目编号")
    private String clinicNo;

    @ExcelProperty("操作时间")
    @DateTimeFormat("yyyy-MM-dd HH:mm:ss")
    private LocalDateTime opTime;

    @ExcelProperty("药房位置")
    private String pharmacyName;

    @ExcelProperty("操作账号")
    private String operatorAccount;

    @ExcelProperty("药品名称")
    private String drugName;
}
