package com.zeroone.star.project.vo.j7.dispense;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

/**
 * <p>
 * 描述：患者药品项目视图对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author zg
 * @version 1.0.0
 */
@Schema(description = "患者药品项目")
@Data
@AllArgsConstructor
@NoArgsConstructor
public class PatientMedicationVO {

    @Schema(description = "发药明细ID", example = "d12345678901234567890")
    private String id;

    @Schema(description = "汇总单号", example = "HZ20240719001")
    private String summaryNo;

    @Schema(description = "患者ID", example = "pat1234567890123456789")
    private String patientId;

    @Schema(description = "患者姓名", example = "张三")
    private String patientName;

    @Schema(description = "床位号", example = "01床")
    private String bedNo;

    @Schema(description = "药品ID", example = "med1234567890123456789")
    private String medicationId;

    @Schema(description = "药品编码", example = "YP0001")
    private String drugCode;

    @Schema(description = "药品名称", example = "阿莫西林胶囊")
    private String drugName;

    @Schema(description = "规格", example = "0.25g*24粒")
    private String spec;

    @Schema(description = "单位", example = "盒")
    private String unit;

    @Schema(description = "应发数量", example = "10.00")
    private java.math.BigDecimal quantity;

    @Schema(description = "实发数量", example = "10.00")
    private java.math.BigDecimal dispenseQuantity;

    @Schema(description = "用法说明", example = "口服，每日三次")
    private String dosageInstruction;

    @Schema(description = "频次编码", example = "TID")
    private String frequencyCode;

    @Schema(description = "批号", example = "BATCH20240701")
    private String lotNumber;
}
