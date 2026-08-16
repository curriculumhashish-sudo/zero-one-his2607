package com.zeroone.star.project.vo.stocktake;

import com.zeroone.star.project.dto.PageDTO;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.util.Collections;
import java.util.List;

/**
 * 盘点单分页响应对象。
 *
 * @author 北海
 * @version 1.0.0
 */
@Data
@Schema(description = "盘点单分页响应对象")
public class StocktakePageVO {

    @Schema(description = "当前页码", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private Long pageIndex;

    @Schema(description = "每页显示最大数据条数", example = "10", requiredMode = Schema.RequiredMode.REQUIRED)
    private Long pageSize;

    @Schema(description = "总条数", example = "100", requiredMode = Schema.RequiredMode.REQUIRED)
    private Long total;

    @Schema(description = "总页数", example = "10", requiredMode = Schema.RequiredMode.REQUIRED)
    private Long pages;

    @Schema(description = "当前页盘点单列表")
    private List<StocktakeListVO> rows;

    public static StocktakePageVO from(PageDTO<StocktakeListVO> page) {
        StocktakePageVO vo = new StocktakePageVO();
        if (page == null) {
            vo.setPageIndex(0L);
            vo.setPageSize(0L);
            vo.setTotal(0L);
            vo.setPages(0L);
            vo.setRows(Collections.emptyList());
            return vo;
        }
        vo.setPageIndex(page.getPageIndex());
        vo.setPageSize(page.getPageSize());
        vo.setTotal(page.getTotal());
        vo.setPages(page.getPages());
        vo.setRows(page.getRows() == null ? Collections.emptyList() : page.getRows());
        return vo;
    }
}
