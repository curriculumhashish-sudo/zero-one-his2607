package com.zeroone.star.project.query.j2.basedata;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import lombok.EqualsAndHashCode;

@Data
@EqualsAndHashCode(callSuper = true)
@Schema(description = "字典类型查询对象")
public class DictionaryTypeQuery extends PageQuery {

    @Schema(description = "关键字，可匹配字典类型名称、编码或描述", example = "体温", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String keyword;

    @Schema(description = "字典类型名称", example = "体温单特殊事件", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String name;
}
