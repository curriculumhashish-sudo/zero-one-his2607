package com.zeroone.star.project.query.j4.equipment;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import lombok.EqualsAndHashCode;

/**
 * 器材分页查询 DTO
 */
@EqualsAndHashCode(callSuper = true)
@Data
@Schema(description = "器材分页查询")
public class EquipmentPageQuery extends PageQuery {

    @Schema(description = "关键字搜索（器材名称/业务编号）", example = "一次性使用无菌注射器")
    private String keyword;

    @Schema(description = "器材分类编码", example = "11")
    private String categoryCode;
}