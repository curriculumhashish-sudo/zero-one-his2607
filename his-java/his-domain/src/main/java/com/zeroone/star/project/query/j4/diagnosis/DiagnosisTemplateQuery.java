package com.zeroone.star.project.query.j4.diagnosis;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

/**
 * <p>
 * 描述：诊断模板分页查询对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@Getter
@Setter
@ToString(callSuper = true)
public class DiagnosisTemplateQuery extends PageQuery {

    /**
     * 绑定类型枚举：个人 / 科室 / 全院
     */
    @Schema(description = "绑定类型枚举：个人 / 科室 / 全院", example = "个人", requiredMode = Schema.RequiredMode.REQUIRED)
    private String bindingEnum;

    /**
     * 模板归属主体（医生 ID / 科室 ID / 全院标识）
     */
    @Schema(description = "模板归属主体（医生ID / 科室ID / 全院标识）", example = "322963602154721280",
            requiredMode = Schema.RequiredMode.REQUIRED)
    private String objectId;
}
