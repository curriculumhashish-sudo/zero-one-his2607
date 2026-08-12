package com.zeroone.star.project.dto.j2.sysmgr;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;
import java.util.List;
/**
 * <p>
 * 描述：租户用户传输数据
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author awei
 * @version 1.0.0
 */
@Data
@AllArgsConstructor
@NoArgsConstructor
@Schema(description = "租户用户")
public class TenantUserDTO {

    @Schema(description = "租户ID", example = "123", requiredMode = Schema.RequiredMode.REQUIRED)
    private String tenantId;

    @Schema(description = "用户名", example = "zhangsan", requiredMode = Schema.RequiredMode.REQUIRED)
    private String username;

    @Schema(description = "姓名", example = "张三", requiredMode = Schema.RequiredMode.REQUIRED)
    private String name;
    @Schema(description = "用户唯一标识", example = "10001")
    private String id;
    @Schema(description = "邮箱", example = "zhangsan@example.com")
    private String email;
    @Schema(description = "手机号", example = "13800138000")
    private String phone;
    @Schema(description = "角色名称列表", example = "\"医生\",\"管理员\"")
    private List<String> roleNames;
}
