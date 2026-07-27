package com.zeroone.star.project.vo.j3.storehouse;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.io.Serializable;

/**
 * 描述：仓库列表显示数据对象
 * @author mie
 * @version 1.0.0
 */
@Schema(description = "仓库列表显示对象")
@Data
public class StoreHouseVO implements Serializable {
    @Schema(description = "仓库ID", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private Long id;

    @Schema(description = "仓库名称", example = "西药库", requiredMode = Schema.RequiredMode.REQUIRED)
    private String name;

    @Schema(description = "仓库类型（仓库/药房/科室药房）", example = "仓库", requiredMode = Schema.RequiredMode.REQUIRED)
    private String type;

    @Schema(description = "上级科室名称", example = "门诊内科", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String superiorName;

    @Schema(description = "是否启用 1启用 0停用", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private Byte isEnabled;
}
