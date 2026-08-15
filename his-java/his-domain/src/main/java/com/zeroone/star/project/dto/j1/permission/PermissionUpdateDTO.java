package com.zeroone.star.project.dto.j1.permission;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.Size;
import lombok.Data;

@Data
@Schema(description = "修改权限数据对象")
public class PermissionUpdateDTO {
    @NotBlank(message = "显示名称不能为空")
    @Size(max = 256, message = "显示名称长度不能超过256个字符")
    @Schema(description = "显示名称", requiredMode = Schema.RequiredMode.REQUIRED)
    private String displayName;

    @Size(max = 256, message = "父权限标识长度不能超过256个字符")
    @Schema(description = "父权限标识")
    private String parentName;

    @Size(max = 1024, message = "描述长度不能超过1024个字符")
    @Schema(description = "描述")
    private String description;
}
