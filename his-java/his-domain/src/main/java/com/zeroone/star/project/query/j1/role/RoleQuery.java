package com.zeroone.star.project.query.j1.role;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.Size;
import lombok.Data;
import lombok.EqualsAndHashCode;

/**
 * 角色分页查询条件
 * <p>
 * 封装角色列表的分页参数和筛选条件
 * </p>
 *
 * @author j1-保温杯热水仙人
 * @since 2026-07-24
 */
@Data
@EqualsAndHashCode(callSuper = true)
@Schema(description = "角色分页查询条件")
public class RoleQuery extends PageQuery {

    @Size(max = 256, message = "角色关键字长度不能超过256个字符")
    @Schema(description = "角色关键字，匹配内部名称或显示名称", example = "医生")
    private String keyword;

    @Schema(description = "是否为默认角色", example = "true")
    private Boolean isDefault;

    @Schema(description = "是否为内置静态角色", example = "false")
    private Boolean isStatic;
}
