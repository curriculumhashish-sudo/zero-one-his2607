package com.zeroone.star.project.dto.j8.setroster;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.io.Serializable;
import java.util.List;

/**
 * <p>
 * 描述：放号策略分页数据VO
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
@Schema(description = "放号策略分页数据")
public class SlotReleaseStrategyPageVO<T> implements Serializable {

    private static final long serialVersionUID = 1L;

    @Schema(description = "当前页数据列表")
    private List<T> content;

    @Schema(description = "数据总条数", example = "100")
    private Long totalCount;

    @Schema(description = "当前页码", example = "1")
    private Long pageIndex;

    @Schema(description = "每页显示条数", example = "10")
    private Long pageSize;
}