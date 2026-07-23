package com.zeroone.star.project.dto.j8.setroster;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.NotNull;
import jakarta.validation.constraints.Min;
import lombok.Data;

import java.io.Serializable;

/**
 * <p>
 * 描述：放号时段模板中的单个时段项
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author Gin
 * @version 1.0.0
 */
@Data
@Schema(description = "放号时段项")
public class SlotTemplateItemDTO implements Serializable {

    private static final long serialVersionUID = 1L;

    @NotBlank(message = "开始时间不能为空")
    @Schema(description = "时段开始时间", example = "08:00", requiredMode = Schema.RequiredMode.REQUIRED)
    private String startTime;

    @NotBlank(message = "结束时间不能为空")
    @Schema(description = "时段结束时间", example = "12:00", requiredMode = Schema.RequiredMode.REQUIRED)
    private String endTime;

    @NotNull(message = "号源容量不能为空")
    @Min(value = 1, message = "号源容量必须大于0")
    @Schema(description = "该时段的号源容量", example = "10", requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer capacity;
}
