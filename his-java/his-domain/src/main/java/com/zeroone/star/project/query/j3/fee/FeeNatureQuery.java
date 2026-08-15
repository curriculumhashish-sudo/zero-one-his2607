package com.zeroone.star.project.query.j3.fee;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

/**
 * <p>
 * 描述：费用性质查询条件
 * @author 白文儿
 * @version 1.0.0
 */
@Getter
@Setter
@ToString
public class FeeNatureQuery extends PageQuery {

    @Schema(description = "关键词（名称或编码）", example = "自费")
    private String keyword;
}
