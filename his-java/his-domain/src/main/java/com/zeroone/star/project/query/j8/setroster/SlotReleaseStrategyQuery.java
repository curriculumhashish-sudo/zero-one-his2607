package com.zeroone.star.project.query.j8.setroster;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.Min;
import jakarta.validation.constraints.Size;
import lombok.Data;

import java.io.Serializable;

/**
 * <p>
 * 描述：放号策略分页查询条件
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author 余悸
 * @version 1.0.0
 */
@Data
@Schema(description = "放号策略分页查询条件")
public class SlotReleaseStrategyQuery implements Serializable {

    private static final long serialVersionUID = 1L;

    @Schema(description = "组织ID", example = "ORG001")
    private String organizationId;

    @Size(max = 50, message = "搜索关键词不能超过50个字符")
    @Schema(
            description = "搜索关键词，匹配策略编码或策略名称",
            example = "提前7天"
    )
    private String keyword;

    @Min(value = 1, message = "页码最小值为1")
    @Schema(description = "查询页码", example = "1")
    private long pageIndex;

    @Min(value = 1, message = "条数最小值为1")
    @Schema(description = "查询条数", example = "10")
    private long pageSize;
}