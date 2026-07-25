package com.zeroone.star.project.query.j3.ward;


import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;


/**
 * <p>
 * 描述：病区查询条件
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author zhu
 * @version 1.0.0
 */
@Data
@Schema(description = "病区查询条件")
public class WardQuery {
    @Schema(description = "唯一标识", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private Long id;
    @Schema(description = "病区名称", example = "内科疗区", requiredMode = Schema.RequiredMode.REQUIRED)
    private String wardName;
}




