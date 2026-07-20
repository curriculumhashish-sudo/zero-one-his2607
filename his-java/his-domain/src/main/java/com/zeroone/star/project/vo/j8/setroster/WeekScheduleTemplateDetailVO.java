package com.zeroone.star.project.vo.j8.setroster;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.time.LocalDate;
import java.util.List;

/**
 * <p>
 * 描述：周排班模板详情
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author 01star
 * @version 1.0.0
 */
@Data
@Schema(description = "周排班模板详情")
public class WeekScheduleTemplateDetailVO {

    @Schema(description = "主键ID", example = "wk-tpl-001")
    private String id;

    @Schema(description = "模板编码", example = "WK-IM-001")
    private String templateCode;

    @Schema(description = "模板名称", example = "内科周排班模板")
    private String templateName;

    @Schema(description = "机构/科室ID", example = "org-001")
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

    @Schema(description = "明细项")
    private List<DetailItemVO> items;

    /**
     * <p>
     * 描述：周排班模板明细项
     * </p>
     * <p>版权：&copy;01星球</p>
     * <p>地址：01星球总部</p>
     *
     * @author 01star
     * @version 1.0.0
     */
    @Data
    @Schema(description = "周排班模板明细项")
    public static class DetailItemVO {

        @Schema(description = "明细主键", example = "item-001")
        private String id;

        @Schema(description = "星期：1=周一...7=周日", example = "1")
        private Integer weekday;

        @Schema(description = "关联日排班模板ID", example = "tpl-001")
        private String dayScheduleTemplateId;

        @Schema(description = "日排班模板名称", example = "内科上午门诊")
        private String dayScheduleTemplateName;

        @Schema(description = "排序号", example = "0")
        private Integer sortNo;

        @Schema(description = "是否启用", example = "true")
        private Boolean enabled;
    }
}
