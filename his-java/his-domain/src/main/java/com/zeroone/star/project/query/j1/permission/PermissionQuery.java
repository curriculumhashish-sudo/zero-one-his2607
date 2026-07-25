package com.zeroone.star.project.query.j1.permission;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.Size;
import lombok.Data;
import lombok.EqualsAndHashCode;

@Data
@EqualsAndHashCode(callSuper = true)
@Schema(description = "权限分页查询条件")
public class PermissionQuery extends PageQuery {
    @Size(max = 256, message = "权限关键字长度不能超过256个字符")
    @Schema(description = "权限关键字，匹配权限标识或显示名称", example = "用户管理")
    private String keyword;
}
