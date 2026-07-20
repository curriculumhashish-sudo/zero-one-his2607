package com.zeroone.star.project.dto.j8.setroster;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import lombok.Data;

/**
 * <p>
 * 描述：新增/编辑日排班模板传输对象
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
    @Schema(description = "模式", example = "医师门诊(MODE-A)", requiredMode = Schema.RequiredMode.REQUIRED)
    private String scheduleMode;

    @NotBlank(message = "号源类型不能为空")
    @Schema(description = "号源类型", example = "普通号", requiredMode = Schema.RequiredMode.REQUIRED)
    private String healthcareServiceId;

    @Schema(description = "医师", example = "doc-001", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String practitionerId;

    @Schema(description = "诊室ID", example = "room-001", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String locationId;

    @Schema(description = "放号时段", example = "每天30个", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String slotProvisionTemplateId;

    @Schema(description = "放号策略(请先在[放号策略]Tab新建(通常不可选))", example = "policy-001", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String defaultReleasePolicyId;

    @Schema(description = "启用", example = "true", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private Boolean enabled;
}
