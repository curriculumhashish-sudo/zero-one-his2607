package com.zeroone.star.project.dto.j1.login.vo;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

@Data
@Schema(description = "科室绑定结果视图对象")
public class DeptBindResultVO {

    @Schema(description = "是否绑定成功", example = "true")
    private Boolean success;

    @Schema(description = "绑定的科室ID", example = "2005")
    private String deptId;

    @Schema(description = "绑定的科室名称", example = "内科")
    private String deptName;

    @Schema(description = "提示信息", example = "科室选择成功")
    private String message;
}
