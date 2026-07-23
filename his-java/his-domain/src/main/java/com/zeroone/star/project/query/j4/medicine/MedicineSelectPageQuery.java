package com.zeroone.star.project.query.j4.medicine;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

/**
 * <p>
 * 描述：选择药品列表条件分页查询对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@Getter
@Setter
@ToString(callSuper = true)
@Schema(description = "选择药品条件分页查询对象")
public class MedicineSelectPageQuery extends PageQuery {

    @Schema(description = "关键字，支持项目编码、名称、拼音码和五笔码")
    private String keyword;

    @Schema(description = "药品分类编码")
    private String categoryCode;
}
