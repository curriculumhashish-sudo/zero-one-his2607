package com.zeroone.star.project.dto.j2.basedata.dictionarydto;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

@Data
@Schema(description = "字典显示对象")
public class DictionaryTypeDTO {

    @Schema(description = "字典类型ID", example = "1")
    private String dictTypeId;

    @Schema(description = "字典类型名称", example = "体温单特殊事件")
    private String dictTypeName;

    @Schema(description = "字典类型标题", example = "special_event_code")
    private String dictTypeCode;
}