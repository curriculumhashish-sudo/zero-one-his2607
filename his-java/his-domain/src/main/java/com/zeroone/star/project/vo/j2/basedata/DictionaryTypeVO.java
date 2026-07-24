package com.zeroone.star.project.vo.j2.basedata;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

@Data
@Schema(description = "字典显示对象")
public class DictionaryTypeVO {

    @Schema(description = "字典类型ID", example = "1")
    private String id;

    @Schema(description = "字典类型名称", example = "体温单特殊事件")
    private String name;

    @Schema(description = "字典类型代码", example = "special_event_code")
    private String code;

    @Schema(description = "排序", example = "1")
    private String order;

    @Schema(description = "字典类型描述", example = "体温单特殊事件")
    private String description;
}
