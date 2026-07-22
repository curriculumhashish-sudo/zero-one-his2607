package com.zeroone.star.project.dto.menu;

import io.swagger.v3.oas.annotations.media.Schema;
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
@Schema(description = "UpdateMenuRequest")
public class MenuUpdateDTO extends MenuCreateDTO {

    @Schema(description = "是否清空父菜单", example = "false")
    private Boolean clearParent;
}
