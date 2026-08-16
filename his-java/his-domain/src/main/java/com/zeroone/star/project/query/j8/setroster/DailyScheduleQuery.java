package com.zeroone.star.project.query.j8.setroster;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * <p>
 * 描述：日排班模板查询条件
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author 墨
 * @version 1.0.0
 */

@Data
@Schema(description = "日排班模板查询条件")
public class DailyScheduleQuery extends PageQuery {

    @Schema(description = "关键词（模糊搜索：编码/名称）", example = "门诊医生")
    private String keyword;

    @Schema(description = "机构/科室ID，为空则查询所有+全院共享模板", example = "322663218269917184")
    private String organizationId;

}
