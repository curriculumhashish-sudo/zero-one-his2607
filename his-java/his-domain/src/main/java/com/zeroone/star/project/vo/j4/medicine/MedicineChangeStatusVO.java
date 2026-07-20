package com.zeroone.star.project.vo.j4.medicine;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

import java.util.List;

@Getter
@Setter
@ToString
@AllArgsConstructor
@Schema(description = "药品状态改变VO（批量）")
public class MedicineChangeStatusVO {
    @Schema(example = "[\"1\"]",defaultValue = "[\"1\"]")
    private List<String> ids;
}
