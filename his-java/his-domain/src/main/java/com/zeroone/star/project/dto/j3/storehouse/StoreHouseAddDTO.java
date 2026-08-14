package com.zeroone.star.project.dto.j3.storehouse;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import lombok.Data;

import java.io.Serializable;

/**
 * 库房药房新增数据对象
 * @author mie
 * @version 1.0.0
 */
@Data
@Schema(description = "库房药房新增数据对象")
public class StoreHouseAddDTO implements Serializable {

    @NotBlank(message = "名称不能为空")
    @Schema(description = "名称", example = "西药库", requiredMode = Schema.RequiredMode.REQUIRED)
    private String name;

    @NotBlank(message = "形态不能为空")
    @Schema(description = "形态枚举(WAREHOUSE/PHARMACY/DEPT_PHARMACY)", example = "WAREHOUSE", requiredMode = Schema.RequiredMode.REQUIRED)
    private String formEnum;

    @Schema(description = "机构ID，不传使用当前登录用户科室", example = "322663105141149696")
    private String organizationId;
}
