package com.zeroone.star.project.dto.j8.setroster;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import lombok.Data;
import lombok.EqualsAndHashCode;

/**
 * <p>
 * 描述：修改放号时段模板DTO
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author Gin
 * @version 1.0.0
 */
@Data
@EqualsAndHashCode(callSuper = true)
@Schema(description = "修改放号时段模板参数")
public class SlotProvisionTemplateModifyDTO extends SlotProvisionTemplateAddDTO {

    @NotBlank(message = "放号时段模板主键ID不能为空")
    @Schema(
            description = "放号时段模板主键ID",
            example = "tpl-001",
            requiredMode = Schema.RequiredMode.REQUIRED
    )
    private String id;
}
