package com.zeroone.star.project.dto.j8.clinics;


import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.Valid;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.NotEmpty;
import lombok.Data;

import java.util.List;

/**
 * <p>
 * 描述：批量放号传输数据
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 友人帐
 * @version 1.0.0
 */
@Data
@Schema(description = "批量放号传输数据DTO")
public class SlotBatchReleaseDTO {
    @NotBlank(message = "机构id不能为空")
    @Schema(description = "机构id")
    private String organizationId;

    @NotBlank(message = "排班id不能为空")
    @Schema(description = "排班id")
    private String scheduleId;

    @NotEmpty(message = "待放号时段列表不能为空，至少传入一个时段")
    @Valid
    @Schema(description = "待放号时段列表")
    private List<SlotItemDTO> slotTemplates;
}
