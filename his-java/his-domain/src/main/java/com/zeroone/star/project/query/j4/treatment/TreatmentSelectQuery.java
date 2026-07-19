package com.zeroone.star.project.query.j4.treatment;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import lombok.EqualsAndHashCode;

@Data
@EqualsAndHashCode(callSuper = true)
@Schema(description = "诊疗项目选择查询对象（用于其他业务模块选择诊疗时的分页搜索）")
public class TreatmentSelectQuery extends PageQuery {

    @Schema(description = "关键词（诊疗名称或编码，模糊查询）", example = "静脉")
    private String keyword;
}
