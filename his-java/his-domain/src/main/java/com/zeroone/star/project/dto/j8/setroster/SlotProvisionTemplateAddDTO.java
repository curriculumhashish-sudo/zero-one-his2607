package com.zeroone.star.project.dto.j8.setroster;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.Valid;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.NotEmpty;
import jakarta.validation.constraints.Size;
import lombok.Data;

import java.util.List;

/**
 * <p>
 * 描述：新增放号时段模板DTO
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author Gin
 * @version 1.0.0
 */
@Data
@Schema(description = "新增放号时段模板参数")
public class SlotProvisionTemplateAddDTO {

    @NotBlank(message = "模板编码不能为空")
    @Size(max = 64, message = "模板编码不能超过64个字符")
    @Schema(
            description = "模板编码",
            example = "SPT000001",
            requiredMode = Schema.RequiredMode.REQUIRED
    )
    private String templateCode;

    @NotBlank(message = "模板名称不能为空")
    @Size(max = 128, message = "模板名称不能超过128个字符")
    @Schema(
            description = "模板名称",
            example = "门诊上午时段",
            requiredMode = Schema.RequiredMode.REQUIRED
    )
    private String templateName;

    @Schema(
            description = "机构/科室ID，空表示全院",
            example = "dept-001"
    )
    private String organizationId;

    @NotEmpty(message = "时段列表不能为空")
    @Valid
    @Size(max = 100, message = "时段数量不能超过100个")
    @Schema(
            description = "时段列表，每个时段包含开始时间、结束时间和号源容量",
            requiredMode = Schema.RequiredMode.REQUIRED
    )
    private List<SlotTemplateItemDTO> slotTemplates;

    @Schema(description = "是否启用", example = "true")
    private Boolean enabled;
}
