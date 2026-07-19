package com.zeroone.star.project.query.j3.ward;

import lombok.Data;
import io.swagger.v3.oas.annotations.media.Schema;

/**
 * <p>
 * 描述：病房查询条件
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author zhu
 * @version 1.0.0
 */
@Data
@Schema(description = "病房查询条件")
public class RoomQuery {
    @Schema(description = "唯一标识", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private Long id;

    @Schema(description = "名称", example = "101", requiredMode = Schema.RequiredMode.REQUIRED)
    private String roomName;
}
