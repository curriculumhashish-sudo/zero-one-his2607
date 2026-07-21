package com.zeroone.star.project.query.j4.treatment;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import lombok.EqualsAndHashCode;

@EqualsAndHashCode(callSuper = true)
@Data
@Schema(description = "诊疗项目分页查询对象")
public class TreatmentQuery extends PageQuery {

    @Schema(description = "关键词（诊疗名称或编码，模糊查询）", example = "CT")
    private String keyword;

    @Schema(description = "目录分类编码", example = "EXAM")
    private String categoryCode;
}
