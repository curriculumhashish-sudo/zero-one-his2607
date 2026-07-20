package com.zeroone.star.project.vo.j7.dispense;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

/**
 * <p>
 * 描述：住院发药汇总单视图对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author zg
 * @version 1.0.0
 */
@Schema(description = "住院发药汇总单")
@Data
@AllArgsConstructor
@NoArgsConstructor
public class DispensingSummaryVO {

    @Schema(description = "汇总单号", example = "HZ20240719001")
    private String summaryNo;

    @Schema(description = "病区ID", example = "loc12345678901234567890")
    private String wardId;

    @Schema(description = "病区名称", example = "内科一病区")
    private String wardName;

    @Schema(description = "药房科室ID", example = "org12345678901234567890")
    private String deptId;

    @Schema(description = "药房名称", example = "住院药房")
    private String deptName;

    @Schema(description = "药品总数量", example = "50.00")
    private java.math.BigDecimal totalQty;

    @Schema(description = "患者数", example = "5")
    private Long patientCount;

    @Schema(description = "状态: PENDING-待发药, DISPENSED-已发药", example = "PENDING")
    private String status;

    @Schema(description = "创建时间", example = "2026-07-19 10:30:00")
    private String createTime;

    @Schema(description = "发药时间", example = "2026-07-19 11:00:00")
    private String dispenseTime;

    @Schema(description = "操作人姓名", example = "药师张三")
    private String operatorName;

    @Schema(description = "截单时间", example = "2026-07-19 12:00:00")
    private String closeTime;

    @Schema(description = "截单人姓名", example = "药师李四")
    private String closerName;
}
