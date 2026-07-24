package com.zeroone.star.project.dto.j2.sysmgr.tenant_userdto;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.util.List;

/**
 * <p>
 * 描述：批量移除租户用户传输数据
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author awei
 * @version 1.0.0
 */
@Data
@Schema(description = "批量移除租户用户")
public class RemoveTenantUserDTO {

    @Schema(description = "当前租户", example = "123", requiredMode = Schema.RequiredMode.REQUIRED)
    private String tenantId;

    @Schema(description = "要移除的用户名列表")
    private List<String> usernames;
}
