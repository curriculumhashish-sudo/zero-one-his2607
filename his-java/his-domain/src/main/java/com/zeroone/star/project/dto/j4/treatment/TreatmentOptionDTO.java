package com.zeroone.star.project.dto.j4.treatment;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;

import java.io.Serializable;

@Getter
@Setter
@NoArgsConstructor
@AllArgsConstructor
@Schema(description = "诊疗下拉选项数据")
public class TreatmentOptionDTO implements Serializable {

    private static final long serialVersionUID = 1L;

    @Schema(description = "选项值")
    private String value;

    @Schema(description = "显示文本")
    private String label;
}
