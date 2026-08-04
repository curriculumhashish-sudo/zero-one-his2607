package com.zeroone.star.project.query.j4.diagnosis;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.Size;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

import java.util.List;

/**
 * 选择诊断目录列表分页查询对象。
 *
 * @author j4组
 * @version 1.0.0
 */
@Getter
@Setter
@ToString(callSuper = true)
@Schema(description = "选择诊断目录列表分页查询对象")
public class DiagnosisSelectPageQuery extends PageQuery {

    @Size(max = 255, message = "模糊查询关键字长度不能超过255个字符")
    @Schema(description = "诊断目录名称或编码关键字", example = "中医")
    private String keyword;

    @Size(max = 50, message = "诊断目录编码长度不能超过50个字符")
    @Schema(description = "诊断目录编码精确筛选", example = "general_dx")
    private String sourceEnum;

    @Schema(description = "前端已选择的诊断ID列表；传入后仅查询这些诊断")
    private List<String> ids;

}
