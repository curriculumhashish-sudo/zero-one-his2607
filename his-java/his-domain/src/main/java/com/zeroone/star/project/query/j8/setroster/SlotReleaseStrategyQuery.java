package com.zeroone.star.project.query.j8.setroster;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.Size;
import lombok.Data;
import lombok.EqualsAndHashCode;

@Data
@EqualsAndHashCode(callSuper = true)
@Schema(description = "放号策略分页查询条件")
public class SlotReleaseStrategyQuery extends PageQuery {

    @Size(max = 50, message = "搜索关键词不能超过50个字符")
    @Schema(
            description = "搜索关键词，匹配策略编码或策略名称",
            example = "提前7天"
    )
    private String keyword;
}
