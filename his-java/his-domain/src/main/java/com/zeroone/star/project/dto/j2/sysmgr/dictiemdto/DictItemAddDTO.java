package com.zeroone.star.project.dto.j2.sysmgr.dictiemdto;

import com.fasterxml.jackson.annotation.JsonAlias;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * <p>
 * 描述：新增租户数据传输对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 阿伟学长
 * @version 1.0.0
 */
@Data
@Schema(description = "新增字典编码数据传输对象")
public class DictItemAddDTO {
    @JsonAlias("dictTypeId")
    @Schema(description = "代码体系ID", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private Long codeSystemId;  // ✅ 新增
    @JsonAlias({"dictCode", "code"})
    @Schema(description = "字典编码", example = "ADMISSION", requiredMode = Schema.RequiredMode.REQUIRED)
    private String dicCode;
    @JsonAlias({"dictName", "display"})
    @Schema(description = "字典名称", example = "入院", requiredMode = Schema.RequiredMode.REQUIRED)
    private String dicName;
    @JsonAlias({"dictDescription", "definition"})
    @Schema(description = "字典描述", example = "无", requiredMode = Schema.RequiredMode.REQUIRED)
    private String dicDescription;
    @JsonAlias({"dictSort", "sortOrder"})
    @Schema(description = "字典排序", example = "1", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private Integer dicSort;
}
