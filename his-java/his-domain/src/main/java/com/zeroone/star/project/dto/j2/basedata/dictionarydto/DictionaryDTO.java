package com.zeroone.star.project.dto.j2.basedata.dictionarydto;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

@Data
@Schema(description = "字典显示对象")
public class DictionaryDTO {

    @Schema(description = "字典类型ID", example = "1")
    private String dictId;

    @Schema(description = "字典类型名称", example = "体温单特殊事件")
    private String dictName;

    @Schema(description = "字典类型代码", example = "special_event_code")
    private String dictCode;

    @Schema(description = "排序", example = "1")
    private String dictOrder;

    @Schema(description = "字典类型描述", example = "体温单特殊事件")
    private String dictDescription;
}