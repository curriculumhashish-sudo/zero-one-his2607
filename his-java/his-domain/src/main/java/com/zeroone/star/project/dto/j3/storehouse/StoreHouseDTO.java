package com.zeroone.star.project.dto.j3.storehouse;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * 描述：库房药房数据对象
 *
 * @author mie
 * @version 1.0.0
 */
@Data
@Schema(description = "库房药房数据对象")
public class StoreHouseDTO {

    @Schema(description = "库房药房名称", example = "西药库", requiredMode = Schema.RequiredMode.REQUIRED)
    private String name;

    @Schema(description = "库房药房类型（仓库/药房/科室药房）", example = "仓库", requiredMode = Schema.RequiredMode.REQUIRED)
    private String type;

    @Schema(description = "上级科室名称", example = "门诊内科", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String superiorName;
}
