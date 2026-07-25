package com.zeroone.star.project.query.menu;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotNull;
import lombok.Data;

/**
 * 树形菜单查询对象
 * @author 是丫丫
 * @date 2026/7/19
 */
@Data
@Schema(description = "树形菜单查询对象")
public class MenuTreeQuery {
    /**
     * 菜单ID
     */
    @NotNull(message = "根菜单ID不能为空")
    @Schema(description = "根菜单ID", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;
}
