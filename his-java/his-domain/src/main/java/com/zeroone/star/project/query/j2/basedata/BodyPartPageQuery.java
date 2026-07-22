package com.zeroone.star.project.query.j2.basedata;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

/**
 * 部位分页查询对象
 */
@Schema(description = "部位分页查询对象")
@Getter
@Setter
@ToString
public class BodyPartPageQuery extends PageQuery {

    @Schema(description = "关键字（部位编码/名称模糊匹配）", example = "头")
    private String keyword;

    @Schema(description = "父级ID（不传查全部）", example = "322663105141149695")
    private String parentId;

    @Schema(description = "状态（ACTIVE-启用、INACTIVE-停用；不传查全部）", example = "ACTIVE", allowableValues = {"ACTIVE", "INACTIVE"})
    private String statusEnum;
}
