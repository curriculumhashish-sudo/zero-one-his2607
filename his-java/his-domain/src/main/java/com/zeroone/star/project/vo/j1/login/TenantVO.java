package com.zeroone.star.project.vo.j1.login;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

@Data
@Schema(description = "租户信息")
public class TenantVO {

    @Schema(description = "租户ID", example = "1")
    private Long tenantId;

    @Schema(description = "租户编码", example = "DEFAULT")
    private String tenantCode;

    @Schema(description = "租户名称", example = "默认租户")
    private String tenantName;

    @Schema(description = "租户描述", example = "系统默认租户，用于单租户或主租户场景")
    private String description;

    @Schema(description = "首字母（用于头像显示）", example = "默")
    private String initial;
}