package com.zeroone.star.project.vo.j6.report;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.util.List;

/**
 * 门诊费用明细分页响应 — 对齐前端 PageOutpatientFeeDetailBackendResponse。
 */
@Data
@Schema(description = "门诊费用明细分页响应")
public class OutpatientFeeDetailsVo {

    @Schema(description = "费用明细行列表")
    private List<FeeDetailVO> content;

    @Schema(description = "总行数", example = "150")
    private Long totalCount;

    @Schema(description = "当前页码", example = "1")
    private Long pageNo;

    @Schema(description = "每页大小", example = "10")
    private Long pageSize;

    @Schema(description = "全量金额合计")
    private FeeSummaryVo summary;
}
