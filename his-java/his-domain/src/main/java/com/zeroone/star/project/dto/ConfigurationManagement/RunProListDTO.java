package com.zeroone.star.project.dto.ConfigurationManagement;

import com.zeroone.star.j5.configmanage.dto.ExecPlaceDTO;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

@Data
@Schema(description = "执行项目列表")
public class RunProListDTO extends ExecPlaceDTO {

    @Schema(description = "项目名称", example = "血液常规检查", requiredMode = Schema.RequiredMode.REQUIRED)
    private String name;

    @Schema(description = "分类", example = "检查", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String classify;

}
