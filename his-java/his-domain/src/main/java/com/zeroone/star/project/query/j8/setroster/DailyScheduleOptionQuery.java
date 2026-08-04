package com.zeroone.star.project.query.j8.setroster;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
/**
 * <p>
 * 描述：日排班模板下拉查询条件
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author 墨
 * @version 1.0.0
 */

@Data
@Schema(description = "日排班模板下拉查询条件")
public class DailyScheduleOptionQuery {

    @Schema(description = "关键词（模糊搜索：编码/名称）", example = "门诊医生")
    private String keyword;

    @Schema(description = "机构/科室ID", example = "322663218269917184")
    private String organizationId;

    @Schema(description = "当前页码（可选，不传则查全部）", example = "1")
    private Long pageIndex;

    @Schema(description = "每页条数（可选，不传则查全部）", example = "200")
    private Long pageSize;
}
