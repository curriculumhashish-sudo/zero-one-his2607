package com.zeroone.star.project.vo.j8.setroster;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.io.Serializable;
import java.util.List;

@Data
@Schema(description = "日排班模板分页查询结果")
public class DailySchedulePageVO<T> implements Serializable {

    @Schema(description = "当前页码", example = "1")
    private Long pageIndex;

    @Schema(description = "每页条数", example = "10", type = "integer", format = "int64")
    private Long pageSize;

    @Schema(description = "总条数", example = "6")
    private Long totalCount;

    @Schema(description = "当前页数据列表")
    private List<T> content;
}
