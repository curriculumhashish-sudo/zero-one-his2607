package com.zeroone.star.project.vo.assistant;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Builder;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.util.List;

/**
 * 通用助手可路由的垂直助手能力。
 */
@Data
@Builder
@NoArgsConstructor
@AllArgsConstructor
@Schema(description = "助手能力")
public class AssistantCapabilityVO {

    @Schema(description = "助手编码", example = "registration")
    private String code;

    @Schema(description = "助手名称", example = "挂号助手")
    private String name;

    @Schema(description = "能力说明")
    private String description;

    @Schema(description = "推荐提问示例")
    private List<String> examples;
}
