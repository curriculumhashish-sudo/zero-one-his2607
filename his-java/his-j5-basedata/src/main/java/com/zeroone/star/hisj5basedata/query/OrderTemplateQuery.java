package com.zeroone.star.hisj5basedata.query;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.*;

/**
 * <p>
 * 组套模板列表查询条件（接口 2）
 * </p>
 * @author keepasdd
 */
@AllArgsConstructor
@Data
@ToString(callSuper = true)
public class OrderTemplateQuery extends PageQuery {

    @Schema(description = "组套种类（tab 切换：GENERAL/TCM/PRICING）")
    private String templateKindEnum = "GENERAL";

    @Schema(description = "关键词（按名称模糊搜索）")
    private String keyword;

    @Schema(description = "绑定类型筛选（PERSONAL/DEPT/GLOBAL）")
    private String bindingEnum;

    @Schema(description = "适用场景筛选（OUTPATIENT/INPATIENT/GENERAL）")
    private String sceneEnum;

    @Schema(description = "状态筛选（DRAFT/ACTIVE/INACTIVE）")
    private String statusEnum;
}
