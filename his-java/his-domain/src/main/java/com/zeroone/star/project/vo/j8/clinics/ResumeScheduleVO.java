package com.zeroone.star.project.vo.j8.clinics;


import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * <p>
 * 描述：恢复开诊响应对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 友人帐
 * @version 1.0.0
 */
@Data
@Schema(description = "恢复开诊响应对象")
public class ResumeScheduleVO {
    @Schema(description = "排班id")
    private String scheduleId;

    @Schema(description = "是否需要重新生成号源")
    private Boolean needReprovision;
}
