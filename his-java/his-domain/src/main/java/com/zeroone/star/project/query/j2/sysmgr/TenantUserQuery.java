package com.zeroone.star.project.query.j2.sysmgr;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * <p>
 * 租户用户查询
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author awei
 * @version 1.0.0
 */
@Data
@Schema(description = "租户用户查询")

public class TenantUserQuery extends PageQuery {

    @Schema(description = "租户ID", example = "123", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String tenantId;

    @Schema(description = "用户名", example = "zhangsan", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String username;

    @Schema(description = "姓名", example = "张三", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String name;
}
