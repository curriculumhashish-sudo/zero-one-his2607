package com.zeroone.star.project.query.j4.diagnosis;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
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
    @Schema(description = "诊断名称（模糊搜索）", example = "高血压")
    private String name;

    /**
     * 拼音检索码（支持模糊搜索）
     */
    @Schema(description = "拼音检索码（模糊搜索）", example = "GXY")
    private String pyStr;

    /**
     * 五笔检索码（支持模糊搜索）
     */
    @Schema(description = "五笔检索码（模糊搜索）", example = "YTD")
    private String wbStr;

    /**
     * 来源枚举：general_dx(西医) / tcm_dx(中医) / tcm_syndrome(中医证候)
     */
    @Schema(description = "来源枚举：general_dx(西医) / tcm_dx(中医) / tcm_syndrome(中医证候)", example = "general_dx")
    private String sourceEnum;

    /**
     * 诊断类型编码
     */
    @Schema(description = "诊断类型编码", example = "1")
    private String typeCode;

    /**
     * 状态枚举：ACTIVE / INACTIVE
     */
    @Schema(description = "状态枚举：ACTIVE / INACTIVE", example = "ACTIVE")
    private String statusEnum;
}
