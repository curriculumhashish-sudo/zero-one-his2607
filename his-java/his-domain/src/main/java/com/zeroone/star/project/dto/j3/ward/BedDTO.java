package com.zeroone.star.project.dto.j3.ward;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * <p>
 * 描述：病床数据传输对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author zhujian
 * @version 1.0.0
 */
@Data
@Schema(description = "病床数据传输对象")
public class BedDTO {
    @Schema(description = "病床名称", example = "1号床", requiredMode = Schema.RequiredMode.REQUIRED)
    private String bedName;
    @Schema(description = "状态 0-停用 1-启用", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer status;
}
