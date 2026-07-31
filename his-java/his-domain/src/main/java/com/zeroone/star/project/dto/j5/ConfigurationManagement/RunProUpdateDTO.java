package com.zeroone.star.project.dto.j5.ConfigurationManagement;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.Size;
import lombok.Data;

/**
 * 执行项目修改DTO
 */
@Data
@Schema(description = "执行项目修改DTO")
public class RunProUpdateDTO {

    @NotBlank(message = "项目ID不能为空")
    @Schema(description = "项目ID", requiredMode = Schema.RequiredMode.REQUIRED, example = "1816800000000000001")
    private String id;

    @Size(max = 100, message = "项目名称不能超过100个字符")
    @Schema(description = "项目名称", example = "血常规")
    private String itemName;

    @Size(max = 50, message = "项目编码不能超过50个字符")
    @Schema(description = "项目编码", example = "CBC001")
    private String itemCode;

    @Schema(description = "执行科室ID", example = "1816800000000000002")
    private String execDeptId;

    @Schema(description = "排序号", example = "1")
    private Integer sortOrder;

    @Schema(description = "是否启用", example = "1")
    private Integer isEnabled;
}
