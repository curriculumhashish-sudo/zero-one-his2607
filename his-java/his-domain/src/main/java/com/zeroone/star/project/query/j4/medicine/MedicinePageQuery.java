package com.zeroone.star.project.query.j4.medicine;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.Size;
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
    @Size(max = 100, message = "关键字长度不能超过100个字符")
    @Schema(description = "查询关键字")
    private String keyword;

    @Size(max = 64, message = "药品分类编码长度不能超过64个字符")
    @Schema(description = "药品分类编码")
    private String categoryCode;

    @Schema(description = "启用状态，true启用、false停用", example = "true")
    private Boolean activeFlag;
}