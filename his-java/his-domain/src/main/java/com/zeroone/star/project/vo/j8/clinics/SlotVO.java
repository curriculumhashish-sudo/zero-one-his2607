package com.zeroone.star.project.vo.j8.clinics;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.time.LocalDateTime;


/**
 * <p>
 * 描述：号源响应对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 友人帐
 * @version 1.0.0
 */
@Data
@Schema(description = "号源响应对象")
public class SlotVO {
    @Schema(description = "号源时段主键id")
    private String slotId;

    @Schema(description = "排班id")
    private String scheduleId;

    @Schema(description = "已预约数量")
    private Integer bookedCount;

    @Schema(description = "号源总容量")
    private Integer capacity;

    @Schema(description = "前端展示状态，如：可约、已满")
    private String displayStatus;

    @Schema(description = "时段结束时间")
    private LocalDateTime endTime;

    @Schema(description = "医疗服务项id")
    private String healthcareServiceId;

    @Schema(description = "医疗服务项名称，例如普通号")
    private String healthcareServiceName;

    @Schema(description = "机构id")
    private String organizationId;

    @Schema(description = "机构名称，例如门诊内科")
    private String organizationName;

    @Schema(description = "是否允许超量放号")
    private Boolean overbookedFlag;

    @Schema(description = "接诊医生id")
    private String practitionerId;

    @Schema(description = "接诊医生姓名")
    private String practitionerName;

    @Schema(description = "剩余可预约数量")
    private Integer remainingCount;

    @Schema(description = "时段开始时间")
    private LocalDateTime startTime;

    @Schema(description = "状态编码，free代表可预约")
    private String statusCode;
}
