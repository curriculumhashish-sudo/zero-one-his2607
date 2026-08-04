package com.zeroone.star.project.dto.j8.setroster;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.io.Serializable;

/**
 * <p>
 * 描述：放号策略ID响应VO
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author 余悸
 * @version 1.0.0
 */
@Data
@NoArgsConstructor
@AllArgsConstructor
@Schema(description = "放号策略ID响应")
public class SlotReleaseStrategyIdVO implements Serializable {

    private static final long serialVersionUID = 1L;

    @Schema(description = "放号策略主键ID", example = "329973511193890816")
    private String id;
}