package com.zeroone.star.project.query.j3.supplier;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.Pattern;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

/**
 * <p>
 * 描述：供应商列表查询条件入参
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author zzcsz
 * @version 1.0.0
 */
@Getter
@Setter
@ToString(callSuper = true)
@Schema(description = "供应商列表查询条件（继承分页基类 PageQuery）")
public class SupplierQuery extends PageQuery {

    @Schema(description = "供应商名称（模糊匹配）", example = "华润")
    private String name;

    @Schema(description = "统一社会信用代码（精确匹配）", example = "91110000100000XXXX")
    private String creditCode;

    @Schema(description = "状态枚举值，可选 ENABLED（启用）或 DISABLED（停用）", example = "ENABLED")
    @Pattern(regexp = "^(ENABLED|DISABLED)$", message = "statusEnum 取值非法，仅支持 ENABLED 或 DISABLED")
    private String statusEnum;

    @Schema(description = "类型枚举值", example = "ENTERPRISE")
    private String typeEnum;
}
