package com.zeroone.star.project.query.j5.bindingMapping;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotNull;
import lombok.Data;
import lombok.EqualsAndHashCode;

/**
 * <p>
 * 描述：用法绑定查询对象
 * </p>
 *
 * @author 扶舒
 * @version 1.0.0
 */
@Data
@EqualsAndHashCode(callSuper = true)
@Schema(description = "用法绑定查询对象")
public class UsageBindingQuery extends PageQuery {

    @NotNull(message = "用法ID不能为空")
    @Schema(description = "用法ID（左侧菜单选中项）", requiredMode = Schema.RequiredMode.REQUIRED)
    private Long usageId;
}
