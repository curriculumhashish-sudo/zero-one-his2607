package com.zeroone.star.project.dto.j6.pharmacy.inventory;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.util.List;

@Data
@Schema(description = "批量停供/取消停供入参DTO")
public class InventoryStatusDTO {

    @Schema(description = "库存商品ID集合", example = "[\"323019059251646464\"]")
    private List<String> inventoryItemIds;
}
