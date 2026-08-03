package com.zeroone.star.project.vo.j8.setroster;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.time.LocalDate;

/**
 * <p>
 * 描述：日排班模板返回对象(分页列表单项)
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author 墨
 * @version 1.0.0
 */

@Data
@Schema(description = "日排班模板返回对象")
public class DailyScheduleVO {

    @Schema(description = "排班ID", example = "328058080426201088")
    private String id;

    @Schema(description = "机构/科室ID", example = "322663218269917184")
    private String organizationId;

    @Schema(description = "模板编码", example = "T2-2011f503dd7e")
    private String templateCode;

    @Schema(description = "模板名称", example = "门诊医生")
    private String templateName;

    @Schema(description = "排班模式", example = "MODE-A")
    private String scheduleMode;

    @Schema(description = "医师ID", example = "322963602326687744")
    private String practitionerId;

    @Schema(description = "诊室ID", example = "room-001")
    private String locationId;

    @Schema(description = "号源类型ID", example = "322957264586018816")
    private String healthcareServiceId;

    @Schema(description = "放号时段模板ID", example = "328057873508601856")
    private String slotProvisionTemplateId;

    @Schema(description = "默认放号策略ID", example = "policy-001")
    private String defaultReleasePolicyId;

    @Schema(description = "生效开始日期", example = "2026-01-01")
    private LocalDate effectiveStart;

    @Schema(description = "生效结束日期", example = "2028-01-01")
    private LocalDate effectiveEnd;

    @Schema(description = "优先级", example = "0")
    private Integer priority;

    @Schema(description = "来源岗位主键（导入追溯）", example = "null")
    private String sourcePractitionerRoleId;

    @Schema(description = "是否启用", example = "true")
    private Boolean enabled;
}
