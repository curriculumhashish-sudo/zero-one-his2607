package com.zeroone.star.project.vo.j7.pharmacy;

import com.baomidou.mybatisplus.core.metadata.IPage;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.util.Collections;
import java.util.List;

/**
 * 药房管理-历史入库单-分页响应对象
 *
 * @author 北海
 * @version 1.0.0
 */
@Data
@Schema(description = "历史入库单分页响应对象")
public class PurchaseInboundPageVO {

    @Schema(description = "当前页单据列表")
    private List<PurchaseInboundListVO> records;

    @Schema(description = "总条数", example = "100")
    private Long total;

    @Schema(description = "每页条数", example = "10")
    private Long size;

    @Schema(description = "当前页码", example = "1")
    private Long current;

    @Schema(description = "总页数", example = "10")
    private Long pages;

    public static PurchaseInboundPageVO from(IPage<PurchaseInboundListVO> page) {
        PurchaseInboundPageVO vo = new PurchaseInboundPageVO();
        if (page == null) {
            vo.setRecords(Collections.emptyList());
            vo.setTotal(0L);
            vo.setSize(0L);
            vo.setCurrent(0L);
            vo.setPages(0L);
            return vo;
        }
        vo.setRecords(page.getRecords() == null ? Collections.emptyList() : page.getRecords());
        vo.setTotal(page.getTotal());
        vo.setSize(page.getSize());
        vo.setCurrent(page.getCurrent());
        vo.setPages(page.getPages());
        return vo;
    }
}
