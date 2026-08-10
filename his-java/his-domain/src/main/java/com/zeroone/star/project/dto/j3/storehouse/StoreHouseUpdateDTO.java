package com.zeroone.star.project.dto.j3.storehouse;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.NotNull;
import lombok.Data;

import java.io.Serializable;

/**
 * 库房药房编辑数据对象
 * @author mie
 * @version 1.0.0
 */
@Data
@Schema(description = "库房药房编辑数据对象")
public class StoreHouseUpdateDTO implements Serializable {

    @NotNull(message = "ID不能为空")
    @Schema(description = "主键ID", example = "322954885380706304", requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;

    @NotBlank(message = "名称不能为空")
    @Schema(description = "名称", example = "西药库", requiredMode = Schema.RequiredMode.REQUIRED)
    private String name;

    @NotBlank(message = "形态不能为空")
    @Schema(description = "形态枚举(WAREHOUSE/PHARMACY/DEPT_PHARMACY)", example = "WAREHOUSE", requiredMode = Schema.RequiredMode.REQUIRED)
    private String formEnum;

    @Schema(description = "运营状态枚举", example = "NOT_IN_USE")
    private String operationalEnum;

    @Schema(description = "模式枚举(INSTANCE/CLASS)", example = "INSTANCE")
    private String modeEnum;

    @Schema(description = "机构ID", example = "322663105141149696")
    private String organizationId;

    @Schema(description = "显示顺序", example = "0")
    private Integer displayOrder;

    @Schema(description = "拼音码")
    private String pyStr;

    @Schema(description = "五笔码")
    private String wbStr;

    @Schema(description = "父节点ID")
    private String parentId;
}
