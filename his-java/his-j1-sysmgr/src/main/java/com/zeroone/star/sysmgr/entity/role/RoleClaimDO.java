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
 * 角色声明数据对象
 * <p>
 * 对应数据库中的 whale_role_claims 表
 * </p>
 *
 * @author j1-保温杯热水仙人
 * @since 2026-07-24
 */
@Getter
@Setter
@ToString
@TableName("whale_role_claims")
@Schema(description = "角色声明数据对象")
public class RoleClaimDO extends BaseDO {

    @Serial
    private static final long serialVersionUID = 1L;

    /**
     * 所属角色标识。
     */
    @TableField("role_id")
    @Schema(description = "所属角色标识", example = "329631932614709248", requiredMode = Schema.RequiredMode.REQUIRED)
    private String roleId;

    /**
     * 声明类型。
     */
    @TableField("claim_type")
    @Schema(description = "声明类型", example = "StaffScope", requiredMode = Schema.RequiredMode.REQUIRED)
    private String claimType;

    /**
     * 声明值。
     */
    @TableField("claim_value")
    @Schema(description = "声明值", example = "DOCTOR_VISIT_ORG")
    private String claimValue;

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
