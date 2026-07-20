package com.zeroone.star.project.dto.j2.sysmgr.rentdto;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * <p>
 * 描述：用户数据传输对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 阿伟学长
 * @version 1.0.0
 */
//补一下，创建时间什么的
@Data
@Schema(description = "用户数据传输对象")
public class RentDTO {
    @Schema(description = "租户名称", example = "01星球", requiredMode = Schema.RequiredMode.REQUIRED)
    private String tenantName;
    @Schema(description = "租户状态", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private String tenantStatus;
    @Schema(description = "租户描述", example = "默认租户", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String tenantDescription;
    @Schema(description = "租户ID", example = "1",requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String tenantId;
    @Schema(description = "租户创建时间", example = "2023-07-01 00:00:00",requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String tenantCreateTime;
    @Schema(description = "租户更新时间", example = "2023-07-01 00:00:00",requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String tenantUpdateTime;
    @Schema(description = "租户删除标志", example = "0",requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String tenantDeleteFlag;
    @Schema(description = "租户删除时间", example = "2023-07-01 00:00:00",requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String tenantDeleteTime;
}
