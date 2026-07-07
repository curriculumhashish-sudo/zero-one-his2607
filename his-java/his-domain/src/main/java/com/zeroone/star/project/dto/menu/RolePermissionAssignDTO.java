package com.zeroone.star.project.dto.menu;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotNull;
import lombok.Data;

import java.util.List;

/**
 * <p>
 * 描述：角色权限分配传输数据，用于为role分配menu权限
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author osillow
 * @version 1.0.0
 * @date 2026/07/18 21:00
 */
@Data
@Schema(description = "角色权限分配传输数据对象")
public class RolePermissionAssignDTO {

    /**
     * 角色ID
     */
    @NotNull(message = "角色ID不能为空")
    @Schema(description = "角色ID", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private Long roleId;

    /**
     * 选中的菜单ID集合
     */
    @Schema(description = "选中的菜单ID集合", example = "[1,2,3]")
    private List<Long> menuIds;
}
