package com.zeroone.star.project.dto.menu;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import lombok.Data;
import lombok.EqualsAndHashCode;

/**
 * <p>
 * 描述：菜单更新传输数据
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author osillow
 * @version 1.0.0
 * @date 2026/07/18 21:00
 */
@Data
@EqualsAndHashCode(callSuper = true)
@Schema(description = "菜单更新请求数据")
public class MenuUpdateDTO extends MenuCreateDTO {

    @NotBlank(message = "菜单ID不能为空")
    @Schema(description = "菜单唯一标识ID", requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;

    @Schema(description = "是否清空父菜单", example = "false")
    private Boolean clearParent;
}
