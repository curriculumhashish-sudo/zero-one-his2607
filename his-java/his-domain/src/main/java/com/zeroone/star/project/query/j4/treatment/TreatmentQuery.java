package com.zeroone.star.project.query.j4.treatment;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.Size;
import lombok.Data;
import lombok.EqualsAndHashCode;

@EqualsAndHashCode(callSuper = true)
@Data
@Schema(description = "诊疗项目分页查询对象")
public class TreatmentQuery extends PageQuery {

    @Size(max = 100, message = "关键词长度不能超过100个字符")
    @Schema(description = "关键词（诊疗名称/编码/拼音码/五笔码，模糊查询）", example = "CT")
    private String keyword;

    @Schema(description = "诊疗分类编码（21-检验/22-检查/23-影像/24-治疗/25-手术/26-护理/31-膳食/其他等）", example = "22")
    private String categoryCode;
}
