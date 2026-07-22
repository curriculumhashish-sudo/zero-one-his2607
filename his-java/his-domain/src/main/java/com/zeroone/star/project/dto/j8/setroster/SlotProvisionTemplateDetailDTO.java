package com.zeroone.star.project.dto.j8.setroster;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import lombok.EqualsAndHashCode;

import java.util.List;

/**
 * <p>
 * 描述：放号时段模板详情DTO
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author Gin
 * @version 1.0.0
 */
@Data
@EqualsAndHashCode(callSuper = true)
@Schema(description = "放号时段模板详情数据")
public class SlotProvisionTemplateDetailDTO extends SlotProvisionTemplateDTO {

    @Schema(description = "时段列表，每个时段包含开始时间、结束时间和号源容量")
    private List<SlotTemplateItemDTO> slotTemplates;
}
