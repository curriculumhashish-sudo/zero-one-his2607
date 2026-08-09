package com.zeroone.star.project.dto.j2.sysmgr.rent;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.time.LocalDateTime;

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
    @Schema(description = "租户描述", example = "系统默认租户", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String description;

    @Schema(description = "租户状态 1启用 0禁用", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer isActive;

    @Schema(description = "扩展属性", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String extraProperties;

    @Schema(description = "创建时间", example = "2026-05-02 22:07:04", requiredMode = Schema.RequiredMode.REQUIRED)
    private LocalDateTime createdAt;

    @Schema(description = "创建人", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private String createdBy;

    @Schema(description = "更新时间", example = "2026-05-02 22:07:04", requiredMode = Schema.RequiredMode.REQUIRED)
    private LocalDateTime updatedAt;

    @Schema(description = "更新人", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private String updatedBy;
}
