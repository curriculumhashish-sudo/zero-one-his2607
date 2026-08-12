package com.zeroone.star.project.dto.j2.sysmgr.tenant_userdto;  // 包名不变！

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;
import java.util.List;

@Data
@AllArgsConstructor
@NoArgsConstructor
@Schema(description = "租户用户")
public class TenantUserDTO {

    @Schema(description = "用户唯一标识", example = "10001")
    private Long id;

    @Schema(description = "租户ID", example = "123")
    private String tenantId;

    @Schema(description = "用户名", example = "zhangsan")
    private String username;

    @Schema(description = "姓名", example = "张三")
    private String name;

    @Schema(description = "邮箱", example = "zhangsan@example.com")
    private String email;

    @Schema(description = "手机号", example = "13800138000")
    private String phone;

    @Schema(description = "角色名称列表")
    private List<String> roleNames;
}