package com.zeroone.star.project.dto.j3.ward;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * <p>
 * 描述：病房添加数据传输对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author zhujian
 * @version 1.0.0
 */
@Data
@Schema(description = "病房添加数据传输对象")
public class RoomAddDTO {
    @Schema(description = "病房名称", example = "302", requiredMode = Schema.RequiredMode.REQUIRED)
    private String roomName;
    @Schema(description = "所属病区", example = "内科疗区")
    private String wardCode;
}
