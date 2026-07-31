package com.zeroone.star.project.query.j5.bindingMapping;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotNull;
import lombok.Data;
import lombok.EqualsAndHashCode;

/**
 * <p>
 * 描述：诊疗绑定查询对象
 * </p>
 *
 * @author 扶舒
 * @version 1.0.0
 */
@Data
@EqualsAndHashCode(callSuper = true)
@Schema(description = "诊疗绑定查询对象")
public class TreatmentBindingQuery extends PageQuery {

    @NotNull(message = "诊疗项目ID不能为空")
    @Schema(description = "诊疗项目ID（左侧菜单选中项）", requiredMode = Schema.RequiredMode.REQUIRED)
    private Long treatmentId;
}
