package com.zeroone.star.project.query.j3.department;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.Pattern;
import lombok.Data;

/**
 * <p>
 * 描述：获取科室名称树请求对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author 胡傲宇
 * @version 1.0.0
 */
@Data
@Schema(description = "科室名称树DTO")
public class DepartmentTreeQuery {
    @Schema(description = "关键词", example = "内科", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String keyword;

    @Schema(description = "类型, 1-医院, 2-科室", example = "1", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    @Pattern(regexp = "^[12]$", message = "类型只能是1-2之间的数字")
    private String typeEnum;

    @Schema(description = "类别, 1-门诊, 2-住院, 3-药房, 4-库房, 5-财务, 6-护士站, 7-管理, 8-后勤支持, 9-其他, 10-医技",
            example = "1", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    @Pattern(regexp = "^[1-10]$", message = "类别只能是1-10之间的数字")
    private String classEnum;

    @Schema(description = "状态, ACTIVE-正常, INACTIVE-停用", example = "ACTIVE", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String statusEnum;
}
