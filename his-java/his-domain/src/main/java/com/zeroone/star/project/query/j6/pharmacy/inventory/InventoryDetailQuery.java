package com.zeroone.star.project.query.j6.pharmacy.inventory;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.util.List;

@Data
@Schema(description = "库存明细分页查询入参")
public class InventoryDetailQuery extends PageQuery {
    @Schema(description = "搜索关键字（药品名称/编码/厂商等模糊搜索）", example = "阿莫西林")
    private String keyword;

    @Schema(description = "药品分类编码集合", example = "[\"0101\",\"0102\"]")
    private List<String> medicationCategoryCodes;

    @Schema(description = "器械分类编码集合", example = "[\"0301\"]")
    private List<String> deviceCategoryCodes;

    @Schema(description = "仓库ID集合", example = "[\"1001\",\"1002\"]")
    private List<String> warehouseIds;

    @Schema(description = "供应商ID集合", example = "[\"2001\"]")
    private List<String> supplierIds;

    @Schema(description = "医保等级编码集合", example = "[\"1\",\"2\"]")
    private List<String> chrgitmLvList;

    @Schema(description = "是否仅零库存 true=只查零库存，false=全部", example = "false")
    private Boolean zeroStockOnly;

    @Schema(description = "近N天过期筛选，代表有效期在N天内", example = "30")
    private Integer expireWithinDays;

    @Schema(description = "库存状态枚举集合（正常/停供等）", example = "[\"NORMAL\",\"STOP_SUPPLY\"]")
    private List<String> inventoryStatusEnums;


}
