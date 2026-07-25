package com.zeroone.star.project.query.j2.workbench;
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
    @Schema(description = "登录用户ID", example = "U001", requiredMode = Schema.RequiredMode.REQUIRED)
    private String userId;


    @Schema(description = "菜单名称搜索关键字", example = "门诊")
    private String keyword;
}