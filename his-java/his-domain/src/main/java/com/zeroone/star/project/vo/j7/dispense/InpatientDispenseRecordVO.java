package com.zeroone.star.project.vo.j7.dispense;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.math.BigDecimal;
import java.time.LocalDateTime;

/**
 * <p>
 * 描述：住院发退药记录显示对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author zhishixuebao
 * @version 1.0.0
 */
@Data
@Schema(description = "住院发退药记录显示对象")
public class InpatientDispenseRecordVO {

    @Schema(description = "发退药记录ID", example = "330000000000000001", requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;

    @Schema(description = "发退药业务编号", example = "FY202607200001", requiredMode = Schema.RequiredMode.REQUIRED)
    private String recordNo;

    @Schema(description = "发退药类型枚举值", example = "DISPENSE", requiredMode = Schema.RequiredMode.REQUIRED)
    private String dispenseType;

    @Schema(description = "记录状态枚举值", example = "COMPLETED", requiredMode = Schema.RequiredMode.REQUIRED)
    private String status;

    @Schema(description = "患者ID", example = "326940849401761792", requiredMode = Schema.RequiredMode.REQUIRED)
    private String patientId;

    @Schema(description = "患者姓名", example = "张三", requiredMode = Schema.RequiredMode.REQUIRED)
    private String patientName;

    @Schema(description = "就诊ID", example = "329874660147597312", requiredMode = Schema.RequiredMode.REQUIRED)
    private String encounterId;

    @Schema(description = "住院号", example = "ZY20260720001", requiredMode = Schema.RequiredMode.REQUIRED)
    private String inpatientNo;

    @Schema(description = "病区ID", example = "322963602326687744")
    private String wardId;

    @Schema(description = "病区名称", example = "内科病区")
    private String wardName;

    @Schema(description = "床号", example = "08-12")
    private String bedNo;

    @Schema(description = "药品医嘱ID", example = "330000000000000101")
    private String medicationRequestId;

    @Schema(description = "处方编号", example = "CF202607200001")
    private String prescriptionNo;

    @Schema(description = "药品ID", example = "330000000000000201", requiredMode = Schema.RequiredMode.REQUIRED)
    private String medicationId;

    @Schema(description = "药品名称", example = "阿莫西林胶囊", requiredMode = Schema.RequiredMode.REQUIRED)
    private String medicationName;

    @Schema(description = "药品规格", example = "0.25g*24粒")
    private String medicationSpecification;

    @Schema(description = "发退药数量", example = "2.00", requiredMode = Schema.RequiredMode.REQUIRED)
    private BigDecimal quantity;

    @Schema(description = "数量单位编码", example = "BOX", requiredMode = Schema.RequiredMode.REQUIRED)
    private String unitCode;

    @Schema(description = "药品批号", example = "LOT20260701")
    private String lotNumber;

    @Schema(description = "药品追溯码", example = "869000000000000001")
    private String traceNo;

    @Schema(description = "发药药师ID", example = "330000000000000301")
    private String pharmacistId;

    @Schema(description = "发药药师姓名", example = "李药师")
    private String pharmacistName;

    @Schema(description = "发退药时间", example = "2026-07-20 10:30:00")
    private LocalDateTime dispenseTime;
}
