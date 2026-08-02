package com.zeroone.star.project.vo.j4.equipment;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

/**
 * 器材分类VO
 *
 * <p>对应术语体系 {@code device_category_code} 下的一个分类项。
 */
@Data
@NoArgsConstructor
@AllArgsConstructor
@Schema(description = "器材分类VO")
public class EquipmentCategoryVO {

    @Schema(description = "分类编码", example = "01")
    private String value;

    @Schema(description = "分类名称", example = "低值耗材")
    private String label;
}