package com.zeroone.star.project.dto.menu;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotNull;
import lombok.Data;

/**
 * <p>
 * 描述：菜单启用传输数据
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author osillow
 * @version 1.0.0
 * @date 2026/07/18 21:00
 */
@Data
@Schema(description = "IdRequest")
public class MenuEditStatusDto {

    /**
     * 菜单ID
     */
    @NotNull(message = "菜单ID不能为空")
    @Schema(description = "菜单ID", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private Long id;

    @NotNull(message = "菜单状态不能为空")
    @Schema(description = "菜单状态", example = "1(启用)/0(禁用)", requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer status;
}
