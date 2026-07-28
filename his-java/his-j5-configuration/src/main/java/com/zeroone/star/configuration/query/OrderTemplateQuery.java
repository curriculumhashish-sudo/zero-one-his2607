package com.zeroone.star.configuration.query;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import lombok.Data;
import lombok.EqualsAndHashCode;
import lombok.NoArgsConstructor;
import lombok.AllArgsConstructor;

/**
 * <p>
 * 组套模板列表查询条件（接口 2）
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author keepasdd
 * @version 1.0.0
 */
@Data
@NoArgsConstructor
@AllArgsConstructor
@EqualsAndHashCode(callSuper = true)
public class OrderTemplateQuery extends PageQuery {

    @NotBlank(message = "组套种类不能为空")
    @Schema(description = "组套种类（tab 切换：GENERAL/TCM/PRICING）", example = "GENERAL")
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