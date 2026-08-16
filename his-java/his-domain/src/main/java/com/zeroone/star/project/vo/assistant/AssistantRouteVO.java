package com.zeroone.star.project.vo.assistant;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Builder;
import lombok.Data;
import lombok.NoArgsConstructor;

/**
 * 通用助手的一条路由执行结果。
 */
@Data
@Builder
@NoArgsConstructor
@AllArgsConstructor
@Schema(description = "助手路由结果")
public class AssistantRouteVO {

    @Schema(description = "执行顺序", example = "1")
    private Integer order;

    @Schema(description = "助手编码", example = "registration")
    private String code;

    @Schema(description = "助手名称", example = "挂号助手")
    private String name;

    @Schema(description = "执行状态", allowableValues = {"COMPLETED", "FAILED"}, example = "COMPLETED")
    private String status;
}
