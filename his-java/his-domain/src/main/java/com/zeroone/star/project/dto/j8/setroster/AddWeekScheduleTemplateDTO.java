package com.zeroone.star.project.dto.j8.setroster;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.NotEmpty;
import lombok.Data;

import java.time.LocalDate;
import java.util.List;

/**
 * <p>
 * 描述：新增周排班模板传输对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author 01star
 * @version 1.0.0
 */
@Data
@Schema(description = "新增周排班模板传输对象")
public class AddWeekScheduleTemplateDTO {

    @NotBlank(message = "模板编码不能为空")
    @Schema(description = "模板编码", example = "WK-IM-001", requiredMode = Schema.RequiredMode.REQUIRED)
    private String templateCode;

    @NotBlank(message = "模板名称不能为空")
    @Schema(description = "模板名称", example = "内科周排班模板", requiredMode = Schema.RequiredMode.REQUIRED)
    private String templateName;

    @NotBlank(message = "出诊科室不能为空")
    @Schema(description = "机构/科室ID", example = "org-001", requiredMode = Schema.RequiredMode.REQUIRED)
    private String organizationId;

    @Schema(description = "无排班时是否允许预约", example = "false")
    private Boolean fallbackBookable;

    @Schema(description = "是否科室默认周模板", example = "false")
    private Boolean isDefault;

    @Schema(description = "是否启用", example = "true")
    private Boolean enabled;

    @Schema(description = "生效日期起", example = "2026-07-01")
    private LocalDate effectiveStart;

    @Schema(description = "生效日期止", example = "2026-12-31")
    private LocalDate effectiveEnd;

    @NotEmpty(message = "周明细不能为空")
    @Schema(description = "明细项（周一~周日）", requiredMode = Schema.RequiredMode.REQUIRED)
    private List<WeekScheduleTemplateItemDTO> items;
}
