package com.zeroone.star.project.query.j8.clinics;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import lombok.EqualsAndHashCode;

/**
 * <p>
 * 描述：日程列表查询参数
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author www
 * @version 1.0.0
 */

@Data
@EqualsAndHashCode(callSuper = true)
@Schema(description = "日程列表查询参数对象")
public class ScheduleListQuery extends PageQuery {

    @Schema(description = "机构标识")
    private String organizationId;

    @Schema(description = "医师ID")
    private String practitionerId;

    @Schema(description = "地点ID")
    private String locationId;

    @Schema(description = "医疗服务ID")
    private String healthcareServiceId;

    @Schema(description = "查询起始日期")
    private String rangeStart;

    @Schema(description = "查询结束日期")
    private String rangeEnd;

    @Schema(description = "年月（如 2026-08）")
    private String yearMonth;
}
