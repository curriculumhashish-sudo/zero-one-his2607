package com.zeroone.star.project.dto.j3.storehouse;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.io.Serializable;
/**
 * 描述：仓库数据对象
 * @author mie
 * @version 1.0.0
 */
@Schema(description = "仓库数据对象")
@Data
public class StoreHouseDTO implements Serializable {
    @Schema(description = "仓库ID", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;

    @Schema(description = "仓库名称", example = "西药库", requiredMode = Schema.RequiredMode.REQUIRED)
    private String name;

    @Schema(description = "仓库类型ID", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private Long type;

    @Schema(description = "上级科室ID", example = "1", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String superiorId;

    @Schema(description = "是否启用 1启用 0停用", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private Byte isEnabled;
}
