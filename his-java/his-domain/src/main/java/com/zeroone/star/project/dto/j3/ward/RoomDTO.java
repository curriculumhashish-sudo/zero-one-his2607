package com.zeroone.star.project.dto.j3.ward;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * <p>
 * 描述：病房数据传输对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author zhujian
 * @version 1.0.0
 */
@Data
@Schema(description = "病房数据传输对象")
public class RoomDTO {
    @Schema(description = "病房名称", example = "302", requiredMode = Schema.RequiredMode.REQUIRED)
    private String roomName;
    @Schema(description = "状态 0-停用 1-启用")
    private Integer status;
}
