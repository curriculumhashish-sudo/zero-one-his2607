package com.zeroone.star.project.vo.j2.sysmgr;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * <p>
 * 描述：租户用户显示数据
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author awei
 * @version 1.0.0
 */
@Data
@Schema(description = "租户用户信息")
public class TenantUserVO {

    @Schema(description = "当前租户", example = "123")
    private String currentTenant;

    @Schema(description = "用户名", example = "zhangsan")
    private String username;

    @Schema(description = "姓名", example = "张三")
    private String name;
}
