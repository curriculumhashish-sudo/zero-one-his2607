package com.zeroone.star.project.dto.j8.setroster;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import lombok.Data;

import java.time.LocalDate;

/**
 * <p>
 * 描述：新增日排班模板传输对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author 墨
 * @version 1.0.0
 */
@Data
@Schema(description = "新增日排班模板传输对象")
public class AddDailyScheduleDTO {

    @NotBlank(message = "名称不能为空")
    @Schema(description = "名称", example = "门诊医生", requiredMode = Schema.RequiredMode.REQUIRED)
    private String templateName;

    @NotBlank(message = "模式不能为空")
    @Schema(description = "模式", example = "MODE-A", requiredMode = Schema.RequiredMode.REQUIRED)
    private String scheduleMode;

    @NotBlank(message = "号源类型不能为空")
    @Schema(description = "号源类型ID", example = "322957264586018816", requiredMode = Schema.RequiredMode.REQUIRED)
    private String healthcareServiceId;

    @Schema(description = "医师ID", example = "doc-001", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String practitionerId;

    @Schema(description = "诊室ID", example = "room-001", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String locationId;

    @Schema(description = "放号时段", example = "328057873508601856", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String slotProvisionTemplateId;

    @Schema(description = "放号策略(请先在[放号策略]Tab新建(通常不可选))", example = "policy-001", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String defaultReleasePolicyId;

    @Schema(description = "启用", example = "true", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private Boolean enabled;

    @Schema(description = "机构/科室ID，为空则全院共享", example = "322663218269917184", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String organizationId;

    @Schema(description = "模板编码", example = "T2-2011f503dd7e")
    private String templateCode;

    @Schema(description = "生效开始日期", example = "2026-01-01")
    private LocalDate effectiveStart;

    @Schema(description = "生效结束日期", example = "2028-01-01")
    private LocalDate effectiveEnd;

    @Schema(description = "优先级", example = "0")
    private Integer priority;


}
