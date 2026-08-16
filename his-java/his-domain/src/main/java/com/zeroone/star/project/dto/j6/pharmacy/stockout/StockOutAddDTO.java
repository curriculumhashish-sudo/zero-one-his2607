package com.zeroone.star.project.dto.j6.pharmacy.stockout;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import lombok.Data;

/**
 * 新增药库出库单 — 简洁请求。
 * 只传源药库、目的药房、制单人等基本上下文，服务端生成空白草稿并返回完整文档结构。
 */
@Data
@Schema(name = "StockOutAddDTO", description = "新增出库单的简洁请求数据传输对象")
public class StockOutAddDTO {

    @NotBlank(message = "源药库不能为空")
    @Schema(
            description = "出库源药库ID",
            example = "322954885380706304",
            requiredMode = Schema.RequiredMode.REQUIRED
    )
    private String sourceLocationId;

    @NotBlank(message = "目的药房不能为空")
    @Schema(
            description = "出库目的药房ID",
            example = "322954949549363200",
            requiredMode = Schema.RequiredMode.REQUIRED
    )
    private String purposeLocationId;

    @Schema(
            description = "制单人员ID（不传则服务端从当前登录用户取）",
            example = "322932096108597248"
    )
    private String practitionerId;

    @Schema(
            description = "关联请领单号，可选",
            example = "QL202608070001"
    )
    private String relatedReqNo;

    @Schema(
            description = "备注",
            example = "门诊药房调拨出库"
    )
    private String remark;
}
