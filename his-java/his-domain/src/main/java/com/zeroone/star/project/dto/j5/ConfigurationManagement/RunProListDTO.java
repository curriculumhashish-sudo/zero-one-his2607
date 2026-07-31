package com.zeroone.star.project.dto.j5.ConfigurationManagement;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * <p>
 * 描述：执行项目列表DTO
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author zeroone
 * @version 1.0.0
 */
@Data
@Schema(description = "执行项目列表DTO")
public class RunProListDTO {

    @Schema(description = "项目ID", example = "1816800000000000001")
    private String id;

    @Schema(description = "项目名称", example = "血常规")
    private String itemName;

    @Schema(description = "项目编码", example = "CBC001")
    private String itemCode;

    @Schema(description = "执行科室ID", example = "1816800000000000002")
    private String execDeptId;

    @Schema(description = "执行科室名称", example = "检验科")
    private String execDeptName;

    @Schema(description = "是否启用", example = "1")
    private Integer isEnabled;
}
