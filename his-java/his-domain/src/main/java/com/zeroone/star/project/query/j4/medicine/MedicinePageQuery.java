package com.zeroone.star.project.query.j4.medicine;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

/**
 * <p>
 * 描述：药品列表条件分页查询对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@Getter
@Setter
@ToString(callSuper = true)
@Schema(description = "药品列表条件分页查询对象")
public class MedicinePageQuery extends PageQuery {

    @Schema(
            description = "关键字，支持编码、通用名称、商品名称、拼音码和五笔码",
            example = "阿莫西林"
    )
    private String keyword;

    @Schema(description = "药品分类编码", example = "2")
    private String categoryCode;

    @Schema(description = "启用状态，true启用、false停用", example = "true")
    private Boolean activeFlag;
}