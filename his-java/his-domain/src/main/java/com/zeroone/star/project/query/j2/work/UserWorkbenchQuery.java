package com.zeroone.star.project.query.j2.work;
import io.swagger.v3.oas.annotations.media.Schema;
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

    @Schema(description = "租户医院ID", example = "001", requiredMode = Schema.RequiredMode.REQUIRED)
    private String tenantId;

    //whale_users
    @Schema(description = "登录用户ID，网关自动注入", requiredMode = Schema.RequiredMode.REQUIRED)
    private String userId;

    // whale_organization_units
    @Schema(description = "筛选指定科室ID，不传取用户绑定全部科室权限")
    private String orgId;
}