package com.zeroone.star.project.vo.j6.report;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.util.List;

/**
 * 库存物资有效期预警分布 — 对齐 J8 ExpireWarningVO 结构（items + unit）。
 */
@Data
@Schema(description = "库存物资有效期预警分布")
public class ExpirationWarningVO {

    @Schema(description = "数据项列表")
    private List<DistributionItemVO> items;

    @Schema(description = "单位", example = "种")
    private String unit;
}
