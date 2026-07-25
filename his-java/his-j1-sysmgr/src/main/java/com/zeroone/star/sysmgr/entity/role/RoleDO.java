package com.zeroone.star.sysmgr.entity.role;

import com.baomidou.mybatisplus.annotation.TableField;
import com.baomidou.mybatisplus.annotation.TableName;
import com.zeroone.star.project.entity.BaseDO;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

import java.io.Serial;
import java.time.LocalDateTime;

/**
 * <p>
 * 描述：系统角色数据对象，对应 whale_roles 表
 * </p>
 * @author j1-保温杯热水仙人
 * @date 2026/7/25
 */
@Getter
@Setter
@ToString
@TableName("whale_roles")
@Schema(description = "系统角色数据对象")
public class RoleDO extends BaseDO {

    @Serial
    private static final long serialVersionUID = 1L;

    /**
     * 角色名称，表内唯一。
     */
    @TableField("name")
    @Schema(description = "角色名称，表内唯一", example = "custom_role_882371e0", requiredMode = Schema.RequiredMode.REQUIRED)
    private String name;

    /**
     * 角色显示名称。
     */
    @TableField("display_name")
    @Schema(description = "角色显示名称", example = "门诊医生")
    private String displayName;

    /**
     * 角色编码。
     */
    @TableField("code")
    @Schema(description = "角色编码", example = "ROLE_CUSTOM_882371E0")
    private String code;

    /**
     * 角色描述。
     */
    @TableField("description")
    @Schema(description = "角色描述", example = "负责门诊接诊及病历处理")
    private String description;

    /**
     * 是否为默认角色。
     */
    @TableField("is_default")
    @Schema(description = "是否为默认角色", example = "false", requiredMode = Schema.RequiredMode.REQUIRED)
    private Boolean isDefault;

    /**
     * 是否为内置静态角色，静态角色不可删除。
     */
    @TableField("is_static")
    @Schema(description = "是否为内置静态角色，静态角色不可删除", example = "false", requiredMode = Schema.RequiredMode.REQUIRED)
    private Boolean isStatic;

    /**
     * 所属租户标识。
     */
    @TableField("tenant_id")
    @Schema(description = "所属租户标识", example = "1")
    private String tenantId;

    /**
     * 删除时间。
     */
    @TableField("deleted_at")
    @Schema(description = "删除时间", example = "2026-07-25T10:30:00")
    private LocalDateTime deletedAt;

    /**
     * 删除人标识。
     */
    @TableField("deleted_by")
    @Schema(description = "删除人标识", example = "1")
    private String deletedBy;
}
