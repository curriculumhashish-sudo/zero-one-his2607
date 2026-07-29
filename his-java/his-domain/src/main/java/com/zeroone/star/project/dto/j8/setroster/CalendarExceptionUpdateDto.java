package com.zeroone.star.project.dto.j8.setroster;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.NotNull;
import lombok.Data;

import java.time.LocalDate;

/**
 * <p>描述: 节日例外-修改数据传输对象</p>
 * <p>版权: &copy;</p>
 * <p>地址: 01</p>
 *
 * @author lolololo
 * @version 1.0.0
 */
@Data
@Schema(description = "节日例外修改对象")
public class CalendarExceptionUpdateDto {

    /**
     * 主键ID
     * 对应数据库: id
     * 注意: 修改操作必须携带ID，用于定位记录
     */
    @NotBlank(message = "主键ID不能为空")
    @Schema(description = "主键ID", example = "1752938475839201234", requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;

    /**
     * 例外类型
     * 对应数据库: exception_type
     * 枚举值: CLOSED, REDUCED, OVERRIDE, EXTRA
     */
    @NotBlank(message = "例外类型不能为空")
    @Schema(description = "例外类型(CLOSED/REDUCED/OVERRIDE/EXTRA)", example = "CLOSED", requiredMode = Schema.RequiredMode.REQUIRED)
    private String exceptionType;

    /**
     * 起始日期
     * 对应数据库: exception_date
     */
    @NotNull(message = "起始日期不能为空")
    @Schema(description = "起始日期", example = "2026-07-14", requiredMode = Schema.RequiredMode.REQUIRED)
    private LocalDate exceptionDate;

    /**
     * 结束日期
     * 对应数据库: range_end
     * 业务逻辑: 若为空，则默认为起始日期（单日）
     */
    @Schema(description = "结束日期(为空则默认为单日)", example = "2026-07-16", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private LocalDate rangeEnd;

    /**
     * 说明/备注
     * 对应数据库: reason
     */
    @Schema(description = "说明/备注", example = "国庆节调休", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String reason;

    /**
     * 是否参与自动生成
     * 对应数据库: auto_apply_on_generate
     * 1: 是, 0: 否
     */
    @Schema(description = "是否参与自动生成(1是 0否)", example = "1", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private Integer autoApplyOnGenerate;
}
