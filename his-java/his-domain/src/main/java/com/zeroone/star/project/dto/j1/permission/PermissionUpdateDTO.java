package com.zeroone.star.project.dto.j1.permission;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import lombok.Data;

@Data
public class PermissionUpdateDTO {
    @NotBlank(message = "显示名称不能为空")
    @Schema(description = "显示名称")
    private String displayName;

    @Schema(description = "父权限标识")
    private String parentName;

    @Schema(description = "描述")
    private String description;
}