package com.zeroone.star.project.query.j8.calendar;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotNull;
import lombok.Data;

import java.time.LocalDate;

/**
 * <p>
 * 描述：号源日历分页查询条件
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j8组
 * @version 1.0.0
 */
@Data
@Schema(description = "号源日历分页查询条件")
public class SlotQuery extends PageQuery {

    @Schema(description = "科室/机构主键", requiredMode = Schema.RequiredMode.REQUIRED, example = "322663218269917186")
    @NotNull(message = "科室ID不能为空")
    private String organizationId;

    @Schema(description = "查询日期（yyyy-MM-dd）", example = "2026-07-19", requiredMode = Schema.RequiredMode.REQUIRED)
    @NotNull(message = "查询日期不能为空")
    private LocalDate date;

    @Schema(description = "医师主键（可选，筛选指定医师的号源）")
    private String practitionerId;

    @Schema(description = "号源状态码（可选）：free=可约 / busy=已满 / busy-unavailable=停诊")
    private String statusCode;
}
