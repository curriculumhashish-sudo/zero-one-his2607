package com.zeroone.star.project.dto.j2.sysmgr.rent;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * <p>
 * 描述：租户列表数据传输对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 鹿
 * @version 1.0.0
 */

@Data
@Schema(description = "租户数据传输对象")
public class RentDTO2 extends RentNameDTO{
    @Schema(description = "租户描述",example = "默认租户",requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String description;
    @Schema(description = "租户状态",example = "1",requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer is_active;
}
