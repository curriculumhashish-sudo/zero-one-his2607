package com.zeroone.star.project.dto.j4.sample;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.NotNull;
import lombok.Data;

/**
 * 样本定义 启用/停用 请求参数
 */
@Data
@Schema(description = "样本定义启停用请求")
public class SpecimenDefinitionStatusDTO {

    @NotBlank(message = "样本定义主键不能为空")
    @Schema(description = "样本定义主键", requiredMode = Schema.RequiredMode.REQUIRED,
            example = "1945012345678901234")
    private String id;

    @NotNull(message = "乐观锁版本号不能为空")
    @Schema(description = "乐观锁版本号（用于并发控制，取查询结果中的 version）",
            requiredMode = Schema.RequiredMode.REQUIRED, example = "0")
    private Integer version;

    @NotBlank(message = "目标状态不能为空")
    @Schema(description = "目标发布状态编码（ACTIVE=启用，INACTIVE=停用，对应 PublicationStatus）",
            requiredMode = Schema.RequiredMode.REQUIRED, example = "INACTIVE")
    private String statusEnum;
}