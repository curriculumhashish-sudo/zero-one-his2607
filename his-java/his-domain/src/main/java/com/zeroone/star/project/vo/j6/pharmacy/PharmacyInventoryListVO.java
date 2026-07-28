package com.zeroone.star.project.vo.j6.pharmacy;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import java.util.List;

@Data
@Schema(description = "药房盘点单据分页列表VO")
public class PharmacyInventoryListVO {
    @Schema(description = "分页数据集合")
    private List<PharmacyInventoryItemVO> content;

    @Schema(description = "总条数", example = "3")
    private Long totalCount;

    @Schema(description = "当前页码", example = "1")
    private long pageIndex;

    @Schema(description = "每页大小", example = "100")
    private long pageSize;
}
