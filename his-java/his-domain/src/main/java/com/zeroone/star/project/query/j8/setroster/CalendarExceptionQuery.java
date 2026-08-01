package com.zeroone.star.project.query.j8.setroster;

import com.zeroone.star.project.query.PageQuery;
import lombok.EqualsAndHashCode;
import lombok.Data;
import io.swagger.v3.oas.annotations.media.Schema;

/**
 * <p>描述: 节日例外查询对象</p>
 * <p>版权: &copy;</p>
 * <p>地址: 01</p>
 *
 * @author lolololo
 * @version 1.0.0
 */
@Data
@EqualsAndHashCode(callSuper = true)
@Schema(description = "节日例外查询对象")
public class CalendarExceptionQuery extends PageQuery {

    @Schema(description = "机构/科室ID", example = "DEPT001", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String organizationId;

    @Schema(description = "起始日期", example = "2026-07-14", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String rangeStart;

    @Schema(description = "结束日期", example = "2026-07-16", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String rangeEnd;

    @Schema(description = "例外类型(CLOSED/REDUCED/OVERRIDE/EXTRA)", example = "CLOSED", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String exceptionType;

    @Schema(description = "关键字(模糊搜索原因说明)", example = "国庆", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String keyword;
}
