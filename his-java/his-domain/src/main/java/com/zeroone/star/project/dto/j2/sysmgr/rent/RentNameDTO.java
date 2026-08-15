package com.zeroone.star.project.dto.j2.sysmgr.rent;


import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
/**
 * <p>
 * 描述: 租户管理数据传输对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 鹿
 * @version 1.0.0
 */
@Data
@Schema(description = "租户名称数据传输对象")
public class RentNameDTO {
    @Schema(description = "租户主键id（提供者键存储值",example = "1",requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;
    @Schema(description = "租户名称",example = "默认用户",requiredMode = Schema.RequiredMode.REQUIRED)
    private String name;
    @Schema(description = "租户编码",example = "0001",requiredMode = Schema.RequiredMode.REQUIRED)
    private String code;
}
