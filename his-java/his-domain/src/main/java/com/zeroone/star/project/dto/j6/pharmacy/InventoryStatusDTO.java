package com.zeroone.star.project.dto.j6.pharmacy;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.util.List;

// 库存明细 停供/取消停供（对应前端 retireInventoryItems/resumeInventoryItems）
// 字段：inventoryItemIds[]
@Data
@Schema(description = "批量停供/取消停供入参DTO")
public class InventoryStatusDTO {

    @Schema(description = "库存商品ID集合", example = "[\"323019059251646464\"]")
    private List<String> inventoryItemIds;
}
