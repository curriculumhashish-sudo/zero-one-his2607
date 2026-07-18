package com.zeroone.star.project.dto.j2.sysmgr.RentDTO;

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
public class RentDTO extends RentAddDTO{
    @Schema(description = "租户ID", example = "1")
    private String tenantId;
    @Schema(description = "租户创建时间", example = "2023-07-01 00:00:00")
    private String tenantCreateTime;
    @Schema(description = "租户更新时间", example = "2023-07-01 00:00:00")
    private String tenantUpdateTime;
    @Schema(description = "租户创建人", example = "admin")
    private String tenantCreator;
    @Schema(description = "租户更新人", example = "admin")
    private String tenantUpdater;
    @Schema(description = "租户删除标志", example = "0")
    private String tenantDeleteFlag;
    @Schema(description = "租户删除时间", example = "2023-07-01 00:00:00")
    private String tenantDeleteTime;
    @Schema(description = "租户删除人", example = "admin")
    private String tenantDeleter;
    @Schema(description = "租户删除人名称", example = "admin")
    private String tenantDeleterName;
}
