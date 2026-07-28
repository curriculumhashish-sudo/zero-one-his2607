package com.zeroone.star.project.query.j3.department;

import io.swagger.v3.oas.annotations.media.Schema;
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

    @Schema(description = "类型", example = "1", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private Integer typeEnum;

    @Schema(description = "类别", example = "1", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private Integer classEnum;

    @Schema(description = "状态", example = "ACTIVE", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String statusEnum;
}
