package com.zeroone.star.project.query.j4.diagnosis;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.Size;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

/**
 * <p>
 * 描述：诊断定义分页查询对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@Getter
@Setter
@ToString(callSuper = true)
public class DiagnosisDefinitionQuery extends PageQuery {

    /**
     * 诊断名称（支持模糊搜索）
     */
    @Size(max = 255, message = "诊断名称长度不能超过255个字符")
    @Schema(description = "诊断名称（模糊搜索）", example = "高血压")
    private String name;

    /**
     * 拼音检索码（支持模糊搜索）
     */
    @Size(max = 100, message = "拼音检索码长度不能超过100个字符")
    @Schema(description = "拼音检索码（模糊搜索）", example = "GXY")
    private String pyStr;

    /**
     * 五笔检索码（支持模糊搜索）
     */
    @Size(max = 100, message = "五笔检索码长度不能超过100个字符")
    @Schema(description = "五笔检索码（模糊搜索）", example = "YTD")
    private String wbStr;

    /**
     * 来源枚举：general_dx(西医) / tcm_dx(中医) / tcm_syndrome(中医证候)
     */
    @Size(max = 50, message = "来源枚举长度不能超过50个字符")
    @Schema(description = "来源枚举：general_dx(西医) / tcm_dx(中医) / tcm_syndrome(中医证候)", example = "general_dx")
    private String sourceEnum;

    /**
     * 诊断类型编码
     */
    @Size(max = 50, message = "诊断类型编码长度不能超过50个字符")
    @Schema(description = "诊断类型编码", example = "1")
    private String typeCode;

    /**
     * 状态枚举：ACTIVE / INACTIVE
     */
    @Size(max = 20, message = "状态枚举长度不能超过20个字符")
    @Schema(description = "状态枚举：ACTIVE / INACTIVE", example = "ACTIVE")
    private String statusEnum;
}
