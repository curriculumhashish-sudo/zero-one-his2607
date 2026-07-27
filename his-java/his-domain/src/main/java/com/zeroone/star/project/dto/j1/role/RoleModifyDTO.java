package com.zeroone.star.project.dto.j1.role;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.Size;
import lombok.Data;

/**
 * 修改角色数据对象
 * <p>
 * 封装修改角色基本信息时允许提交的数据
 * </p>
 *
 * @author j1-保温杯热水仙人
 * @since 2026-07-24
 */
@Data
@Schema(description = "修改角色数据对象")
public class RoleModifyDTO {

    @NotBlank(message = "角色编号不能为空")
    @Schema(description = "角色编号", example = "329631932614709248", requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;

    @NotBlank(message = "角色显示名称不能为空")
    @Size(max = 256, message = "角色显示名称长度不能超过256个字符")
    @Schema(description = "角色显示名称", example = "门诊医生", requiredMode = Schema.RequiredMode.REQUIRED)
    private String displayName;

    @Schema(description = "角色描述", example = "负责门诊接诊、病历及处方处理")
    private String description;

    @Schema(description = "是否为默认角色；为空时保持原值", example = "false")
    private Boolean isDefault;
}
