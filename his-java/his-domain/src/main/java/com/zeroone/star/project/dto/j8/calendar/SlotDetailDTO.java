package com.zeroone.star.project.dto.j8.calendar;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import lombok.EqualsAndHashCode;

/**
 * <p>
 * 描述：号源详情 DTO（比列表多备注）
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j8组
 * @version 1.0.0
 */
@Data
@EqualsAndHashCode(callSuper = true)
@Schema(description = "号源详情数据")
public class SlotDetailDTO extends SlotDTO {

    @Schema(description = "备注", example = "该号源为演示号源")
    private String commentText;
}
