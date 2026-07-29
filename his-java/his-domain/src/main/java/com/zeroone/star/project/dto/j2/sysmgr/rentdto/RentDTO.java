package com.zeroone.star.project.dto.j2.sysmgr.rentdto;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * <p>
 * 描述：租户修改数据传输对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 阿伟学长
 * @version 1.0.0
 */
@Data
@Schema(description = "租户修改数据传输对象")
public class RentDTO {
    @Schema(description = "租户ID", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;

    @Schema(description = "租户名称", example = "01星球", requiredMode = Schema.RequiredMode.REQUIRED)
    private String tenantName;

    @Schema(description = "租户状态", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private String tenantStatus;

    @Schema(description = "租户描述", example = "默认租户", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String tenantDescription;
}