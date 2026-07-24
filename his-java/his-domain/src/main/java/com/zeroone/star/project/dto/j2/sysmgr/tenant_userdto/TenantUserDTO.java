package com.zeroone.star.project.dto.j2.sysmgr.tenant_userdto;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

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

    @Schema(description = "当前租户", example = "123")
    private String tenantId;

    @Schema(description = "用户名", example = "zhangsan")
    private String username;

    @Schema(description = "姓名", example = "张三")
    private String name;
}
