package com.zeroone.star.project.vo.j6.pharmacy.stockout;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.math.BigDecimal;
import java.time.LocalDateTime;

@Data
@Schema(description = "出库单列表行")
public class StockOutListVO {

    @Schema(description = "业务单号", example = "CSO2026-07-190004", requiredMode = Schema.RequiredMode.REQUIRED)
    private String busNo;

    @Schema(description = "单据类型枚举", example = "CABINET_STOCK_OUT", requiredMode = Schema.RequiredMode.REQUIRED)
    private String typeEnum;

    @Schema(description = "状态枚举", example = "AGREE", requiredMode = Schema.RequiredMode.REQUIRED)
    private String statusEnum;

    @Schema(description = "状态文本", example = "同意", requiredMode = Schema.RequiredMode.REQUIRED)
    private String statusEnumText;

    @Schema(description = "源药库ID", example = "3229548576228024232", requiredMode = Schema.RequiredMode.REQUIRED)
    private String sourceLocationId;

    @Schema(description = "源药库名称", example = "门诊西药房", requiredMode = Schema.RequiredMode.REQUIRED)
    private String sourceLocationName;

    @Schema(description = "目的药房ID", example = "3229549549549363200", requiredMode = Schema.RequiredMode.REQUIRED)
    private String purposeLocationId;

    @Schema(description = "目的药房名称", example = "门诊西药房", requiredMode = Schema.RequiredMode.REQUIRED)
    private String purposeLocationName;

    @Schema(description = "申请时间", example = "2026-07-19 18:35:38", requiredMode = Schema.RequiredMode.REQUIRED)
    private LocalDateTime applyTime;

    @Schema(description = "审批时间", example = "2026-07-19 18:35:42", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private LocalDateTime approvalTime;

    @Schema(description = "发生时间", example = "2026-07-19 18:35:22", requiredMode = Schema.RequiredMode.REQUIRED)
    private LocalDateTime occurrenceTime;

    @Schema(description = "申请人ID", example = "380791351297856608", requiredMode = Schema.RequiredMode.REQUIRED)
    private String applicantId;

    @Schema(description = "申请人名称", example = "admin", requiredMode = Schema.RequiredMode.REQUIRED)
    private String creatorName;

    @Schema(description = "审批人ID", example = "380791351297856608", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String approverId;

    @Schema(description = "审批人名称", example = "admin", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String approverName;

    @Schema(description = "总金额", example = "6.990000", requiredMode = Schema.RequiredMode.REQUIRED)
    private BigDecimal totalAmount;

    @Schema(description = "原业务单号", example = "null", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String originalBusNo;

}
