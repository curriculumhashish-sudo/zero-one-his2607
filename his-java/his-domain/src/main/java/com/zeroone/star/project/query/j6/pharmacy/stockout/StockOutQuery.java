package com.zeroone.star.project.query.j6.pharmacy.stockout;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.Pattern;
import lombok.Data;
import org.hibernate.validator.constraints.Length;

@Data
@Schema(description = "出库单列表查询参数")
public class StockOutQuery extends PageQuery {
    @Schema(description = "源药库ID", example = "322954885380706304", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    @Length(max = 50, message = "源药库ID长度不能超过50")
    private String sourceLocationId;

    @Schema(description = "目的药房ID", example = "322954949549363200", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    @Length(max = 50, message = "目的药房ID长度不能超过50")
    private String purposeLocationId;

    @Schema(description = "关键字（匹配单号）", example = "CSO202608070001", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String keyword;

    @Schema(description = "申请时间起", example = "2026-07-01 00:00:00", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    @Pattern(regexp = "^\\d{4}-\\d{2}-\\d{2} \\d{2}:\\d{2}:\\d{2}$", message = "申请时间起格式必须为yyyy-MM-dd HH:mm:ss")
    private String applyTimeFrom;

    @Schema(description = "申请时间止", example = "2026-07-31 23:59:59", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    @Pattern(regexp = "^\\d{4}-\\d{2}-\\d{2} \\d{2}:\\d{2}:\\d{2}$", message = "申请时间止格式必须为yyyy-MM-dd HH:mm:ss")
    private String applyTimeTo;
}
