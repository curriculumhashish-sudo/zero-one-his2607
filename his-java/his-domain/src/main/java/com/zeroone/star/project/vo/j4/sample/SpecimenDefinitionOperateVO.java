package com.zeroone.star.project.vo.j4.sample;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * 样本定义 启停用/删除 操作结果
 */
@Data
@Schema(description = "样本定义操作结果响应")
public class SpecimenDefinitionOperateVO {

    @Schema(description = "样本定义主键", example = "1945012345678901234")
    private String id;

    @Schema(description = "操作后的状态编码（删除操作时可为空）", example = "INACTIVE")
    private String statusEnum;

    @Schema(description = "操作后的最新版本号", example = "1")
    private Integer version;
}