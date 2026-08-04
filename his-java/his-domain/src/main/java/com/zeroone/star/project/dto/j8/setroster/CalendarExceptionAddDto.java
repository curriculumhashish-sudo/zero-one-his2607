package com.zeroone.star.project.dto.j8.setroster;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.NotNull;
import lombok.Data;

import java.time.LocalDate;

/**
 * <p>描述: 节日例外数据传输对象</p>
 * <p>版权: &copy;</p>
 * <p>地址: 01</p>
 *
 * @author lolololo
 * @version 1.0.0
 */
@Data
@Schema(description = "节日例外DTO")
public class CalendarExceptionAddDto {

    /**
     * 例外类型
     * 枚举值: CLOSED, REDUCED, OVERRIDE, EXTRA
     */
    @NotBlank(message = "例外类型不能为空")
    @Schema(description = "例外类型(CLOSED/REDUCED/OVERRIDE/EXTRA)", example = "CLOSED", requiredMode = Schema.RequiredMode.REQUIRED)
    private String exceptionType;

    /**
     * 起始日期
     */
    @NotNull(message = "起始日期不能为空")
    @Schema(description = "起始日期", example = "2026-07-14", requiredMode = Schema.RequiredMode.REQUIRED)
    private LocalDate exceptionDate;

    /**
     * 结束日期（为空则默认为单日）
     */
    @Schema(description = "结束日期(为空则默认为单日)", example = "2026-07-16", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private LocalDate rangeEnd;

    /**
     * 机构/科室ID（空=全院）
     */
    @Schema(description = "机构/科室ID", example = "DEPT001", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String organizationId;

    /**
     * REDUCED/OVERRIDE/EXTRA 时关联的排班模板ID
     */
    @Schema(description = "关联排班模板ID", example = "TPL001", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String overrideScheduleTemplateId;

    /**
     * 说明/备注
     */
    @Schema(description = "说明/备注", example = "国庆节调休", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String reason;

    /**
     * 从模板生成排班时是否自动应用
     */
    @Schema(description = "是否参与自动生成", example = "true", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private Boolean autoApplyOnGenerate;
}
