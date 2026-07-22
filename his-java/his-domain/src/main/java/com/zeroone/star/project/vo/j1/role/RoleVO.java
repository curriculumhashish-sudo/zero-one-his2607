package com.zeroone.star.project.vo.j1.role;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.time.LocalDateTime;

/**
 * 角色显示对象。
 *
 * @author j1-保温杯热水仙人
 * @version 1.0.0
 */
@Data
@Schema(description = "角色显示对象")
public class RoleVO {

    @Schema(description = "角色编号", example = "329631932614709248", requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;

    @Schema(description = "角色内部名称", example = "custom_role_882371e0", requiredMode = Schema.RequiredMode.REQUIRED)
    private String name;

    @Schema(description = "角色显示名称", example = "门诊医生", requiredMode = Schema.RequiredMode.REQUIRED)
    private String displayName;

    @Schema(description = "角色编码", example = "ROLE_CUSTOM_882371E0", requiredMode = Schema.RequiredMode.REQUIRED)
    private String code;

    @Schema(description = "角色描述", example = "负责门诊接诊及病历处理")
    private String description;

    @Schema(description = "是否为默认角色", example = "false", requiredMode = Schema.RequiredMode.REQUIRED)
    private Boolean isDefault;

    @Schema(description = "是否为内置静态角色；静态角色不可删除", example = "false", requiredMode = Schema.RequiredMode.REQUIRED)
    private Boolean isStatic;

    @Schema(description = "租户编号", example = "1")
    private String tenantId;

    @Schema(description = "创建时间", example = "2026-07-18T10:30:00", requiredMode = Schema.RequiredMode.REQUIRED)
    private LocalDateTime createdAt;

    @Schema(description = "最后更新时间", example = "2026-07-18T12:00:00")
    private LocalDateTime updatedAt;

    @Schema(description = "创建人编号", example = "1")
    private String createdBy;

    @Schema(description = "最后更新人编号", example = "1")
    private String updatedBy;
}
