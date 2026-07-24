package com.zeroone.star.project.query.j4.diagnosis;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

/**
 * 选择诊断列表分页查询对象。
 *
 * @author j4组
 * @version 1.0.0
 */
@Getter
@Setter
@ToString(callSuper = true)
@Schema(description = "选择诊断列表分页查询对象")
public class DiagnosisSelectPageQuery extends PageQuery {

    @Schema(description = "模糊查询关键字，支持诊断名称、编码、拼音码和五笔码", example = "高血压")
    private String keyword;

    @Schema(description = "诊断目录编码", example = "general_dx")
    private String sourceEnum;
}
