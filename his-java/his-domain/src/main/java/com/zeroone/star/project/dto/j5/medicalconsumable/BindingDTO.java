package com.zeroone.star.project.dto.j5.medicalconsumable;

import io.swagger.v3.oas.annotations.media.Schema;

/**
 * <p>
 * 描述：绑定新增数据
 * </p>
 *
 * @author 扶舒
 * @version 1.0.0
 */
public class BindingDTO {
    @Schema(description = "绑定目标类型")
    private String targetType;
    @Schema(description = "绑定目标名称")
    private String targetName;
    @Schema(description = "数量")
    private Integer quantity;
    @Schema(description = "状态")
    private Integer status;
}