package com.zeroone.star.project.query.j2.workbench;
import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import lombok.Data;

import java.io.Serializable;

/**
 * <p>
 * 描述: 工作台日常功能对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 鹿
 * @version 1.0.0
 */
@Data
@Schema(description = "获取用户工作台菜单查询条件")
public class UserWorkbenchQuery implements Serializable {

    @NotBlank(message = "租户ID不能为空")
    @Schema(description = "租户医院ID", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private String tenantId;

    @Schema(description = "登录用户ID（不传则从JWT解析）", example = "1")
    private String userId;

    @Schema(description = "菜单名称搜索关键字", example = "门诊")
    private String keyword;
}