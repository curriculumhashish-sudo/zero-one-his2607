package com.zeroone.star.project.query.j5.bindingMapping;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import lombok.Data;
import lombok.EqualsAndHashCode;

/**
 * <p>
 * 描述：用法绑定查询对象（映射cus_service_location_template表）
 * </p>
 *
 * @author 扶舒
 * @version 1.0.0
 */
@Data
@EqualsAndHashCode(callSuper = true)
@Schema(description = "用法绑定查询对象")
public class UsageBindingQuery extends PageQuery {

    @NotBlank(message = "项目定义ID不能为空")
    @Schema(description = "项目定义ID（左侧菜单选中项）", requiredMode = Schema.RequiredMode.REQUIRED)
    private String itemDefinitionId;

    @Schema(description = "机构ID")
    private String organizationId;

    @Schema(description = "分配分类编码")
    private String distributionCategoryCode;
}
