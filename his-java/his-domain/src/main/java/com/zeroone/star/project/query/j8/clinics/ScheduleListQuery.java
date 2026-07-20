package com.zeroone.star.project.query.j8.clinics;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * <p>
 * 描述：日期查询参数
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author www
 * @version 1.0.0
 */

@Data
@Schema(description = "日程查询参数对象")
public class ScheduleListQuery extends PageQuery {
    @Schema(description = "科室", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String departmentId;
    @Schema(description = "医师", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String doctorId;
    @Schema(description = "诊室", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String clinicRoomId;
    @Schema(description = "号源类型:便民号,普通号,专家号,急诊号", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String slotType;
    @Schema(description = "视图模式:周 0,月 1", requiredMode = Schema.RequiredMode.REQUIRED)
    private String viewMode;

}
