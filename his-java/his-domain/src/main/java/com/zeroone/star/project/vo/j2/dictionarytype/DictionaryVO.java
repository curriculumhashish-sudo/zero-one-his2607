package com.zeroone.star.project.vo.j2.dictionarytype;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * 字典项操作结果。
 */
@Data
@Schema(description = "字典项操作结果")
public class DictionaryVO {

    @Schema(description = "字典项ID", example = "900010001", requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;

    @Schema(description = "所属字典类型ID", example = "900010", requiredMode = Schema.RequiredMode.REQUIRED)
    private String codeSystemId;

    @Schema(description = "字典编码", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private String code;

    @Schema(description = "字典名称", example = "一般医疗服务费", requiredMode = Schema.RequiredMode.REQUIRED)
    private String display;

    @Schema(description = "字典描述", example = "一般医疗服务费用", requiredMode = Schema.RequiredMode.REQUIRED)
    private String definition;

    @Schema(description = "排序号，数值越小越靠前", example = "1")
    private Long sortOrder;

    @Schema(description = "是否启用", example = "true", requiredMode = Schema.RequiredMode.REQUIRED)
    private Boolean active;
}
