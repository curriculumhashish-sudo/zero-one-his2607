package com.zeroone.star.project.dto.j2.basedata.enumdto;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.*;
import lombok.AllArgsConstructor;
import lombok.Data;

@Data
@AllArgsConstructor
@Schema(description = "枚举值简单信息")
public class EnumSimpleDTO {

    @Schema(description = "枚举ID", example = "exdat1234123047", requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;

    @Schema(description = "枚举类型ID，和EnumType的id关联", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private String enumTypeId;

    @Schema(description = "枚举键", example = "enumKey", requiredMode = Schema.RequiredMode.REQUIRED)
    private String enumKey;

    @Schema(description = "枚举码", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer enumCode;

    @Schema(description = "标签，显示的名称", example = "男性", requiredMode = Schema.RequiredMode.REQUIRED)
    private String display;
}
