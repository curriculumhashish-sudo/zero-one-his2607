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
@Schema(description = "删除药品VO")
public class MedicineDeleteVO {
    @Schema(description = "成功删除的药品id列表",example ="[\"1\"]",defaultValue = "[\"1\"]")
    private List<String> ids;
}
