package com.zeroone.star.project.vo.j8.setroster;

import com.fasterxml.jackson.annotation.JsonFormat;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.time.LocalDate;

/**
 * <p>描述: 节日例外-响应视图对象</p>
 * <p>版权: &copy;</p>
 * <p>地址: 01</p>
 *
 * @author lolololo
 * @version 1.0.0
 */
@Data
@Schema(description = "节日例外响应对象")
public class CalendarExceptionVo {

    /**
     * 主键ID
     */
    @Schema(description = "主键ID", example = "1752938475839201234")
    private Long id;

    /**
     * 机构/科室；空=全院
     */
    @Schema(description = "机构/科室ID", example = "DEPT001")
    private String organizationId;

    /**
     * 起始日期
     */
    @JsonFormat(pattern = "yyyy-MM-dd")
    @Schema(description = "起始日期", example = "2026-07-14")
    private LocalDate exceptionDate;

    /**
     * 结束日期
     */
    @JsonFormat(pattern = "yyyy-MM-dd")
    @Schema(description = "结束日期", example = "2026-07-16")
    private LocalDate rangeEnd;

    /**
     * 例外类型编码
     */
    @Schema(description = "例外类型编码(CLOSED/REDUCED/OVERRIDE/EXTRA)", example = "CLOSED")
    private String exceptionType;

    /**
     * REDUCED/OVERRIDE/EXTRA 时关联 T2 排班模板
     */
    @Schema(description = "关联排班模板ID", example = "TPL001")
    private String overrideScheduleTemplateId;

    /**
     * 说明/备注
     */
    @Schema(description = "说明/备注", example = "国庆节调休")
    private String reason;

    /**
     * 从模板生成排班时是否自动应用
     */
    @Schema(description = "是否参与自动生成", example = "true")
    private Boolean autoApplyOnGenerate;
}
