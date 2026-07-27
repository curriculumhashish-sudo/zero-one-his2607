package com.zeroone.star.project.dto.j1.role;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.Size;
import lombok.Data;

/**
 * 新增角色数据对象
 * <p>
 * 封装创建自定义角色时允许提交的基本信息
 * </p>
 *
 * @author j1-保温杯热水仙人
 * @since 2026-07-24
 */
@Data
@Schema(description = "新增角色数据对象")
public class RoleAddDTO {

    @NotBlank(message = "角色显示名称不能为空")
    @Size(max = 256, message = "角色显示名称长度不能超过256个字符")
    @Schema(description = "角色显示名称", example = "门诊医生", requiredMode = Schema.RequiredMode.REQUIRED)
    private String displayName;

    @Schema(description = "角色描述", example = "负责门诊接诊及病历处理")
    private String description;

    @Schema(description = "是否为默认角色", example = "false", defaultValue = "false")
    private Boolean isDefault = false;
}
