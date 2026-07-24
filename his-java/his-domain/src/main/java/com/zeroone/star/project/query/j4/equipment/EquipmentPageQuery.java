package com.zeroone.star.project.query.j4.equipment;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.Size;
import lombok.Data;
import lombok.EqualsAndHashCode;

/**
 * 器材分页查询 DTO
 */
@EqualsAndHashCode(callSuper = true)
@Data
@Schema(description = "器材分页查询")
public class EquipmentPageQuery extends PageQuery {

    @Size(max = 100, message = "关键字长度不能超过100")
    @Schema(description = "关键字搜索（器材名称/业务编号）", example = "一次性使用无菌注射器")
    private String keyword;

    @Size(max = 50, message = "分类编码长度不能超过50")
    @Schema(description = "器材分类编码", example = "11")
    private String categoryCode;
}