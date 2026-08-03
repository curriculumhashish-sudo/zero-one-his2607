package com.zeroone.star.project.dto.j8.setroster;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.io.Serializable;

/**
 * <p>
 * 描述：放号策略名称DTO
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author 余悸
 * @version 1.0.0
 */
@Data
@Schema(description = "放号策略名称数据")
public class SlotReleaseStrategyNameDTO implements Serializable {

    private static final long serialVersionUID = 1L;

    @Schema(description = "放号策略主键ID", example = "329973511193890816")
    private String id;

    @Schema(description = "放号策略名称", example = "提前七天预约")
    private String strategyName;
}
