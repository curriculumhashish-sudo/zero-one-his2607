package com.zeroone.star.project.vo.j1.role;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * 角色名称选项显示对象。
 *
 * @author j1-保温杯热水仙人
 * @version 1.0.0
 */
@Data
@Schema(description = "角色名称选项显示对象")
public class RoleNameVO {

    @Schema(description = "角色编号", example = "329631932614709248", requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;

    @Schema(description = "角色显示名称", example = "门诊医生", requiredMode = Schema.RequiredMode.REQUIRED)
    private String displayName;
}
