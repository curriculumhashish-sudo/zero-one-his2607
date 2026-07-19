package com.zeroone.star.project.vo.j1.login;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

@Data
@Schema(description = "科室信息视图对象")
public class DepartmentVO {

    @Schema(description = "科室ID", example = "2005")
    private String deptId;

    @Schema(description = "科室名称", example = "内科")
    private String deptName;

    @Schema(description = "科室编码", example = "NK001")
    private String deptCode;
}
