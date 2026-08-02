package com.zeroone.star.project.vo.j7.pharmacy;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

@Data
@Schema(description = "单据删除结果")
public class SupplyDocumentDeleteVO {

    @Schema(description = "单据号")
    private String busNo;

    @Schema(description = "删除行数")
    private int deletedLines;

    @Schema(description = "解锁库存锁记录数")
    private int unlockedLocks;
}
