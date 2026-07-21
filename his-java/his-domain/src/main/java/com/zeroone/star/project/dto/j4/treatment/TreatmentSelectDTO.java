package com.zeroone.star.project.dto.j4.treatment;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Getter;
import lombok.Setter;

import java.io.Serializable;

@Getter
@Setter
@Schema(description = "诊疗项目选择数据（其他业务选择诊疗时使用）")
public class TreatmentSelectDTO implements Serializable {

    private static final long serialVersionUID = 1L;

    @Schema(description = "主键ID")
    private String id;

    @Schema(description = "业务编号（编码）", example = "CG001")
    private String busNo;

    @Schema(description = "诊疗项目名称", example = "血常规检查")
    private String name;
}
