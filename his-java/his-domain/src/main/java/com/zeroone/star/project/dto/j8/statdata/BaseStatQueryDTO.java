package com.zeroone.star.project.dto.j8.statdata;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * 数据统计接口统一请求参数
 */
@Data
@Schema(description = "统计查询基础参数")
public class BaseStatQueryDTO {

    @Schema(description = "开始日期", example = "2026-07-01")
    private String dateFrom;

    @Schema(description = "结束日期", example = "2026-07-31")
    private String dateTo;

    @Schema(description = "粒度：day/month/year", example = "day")
    private String granularity;

    @Schema(description = "机构ID")
    private String organizationId;
}
