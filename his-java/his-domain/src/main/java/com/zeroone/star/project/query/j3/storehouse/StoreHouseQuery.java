package com.zeroone.star.project.query.j3.storehouse;

import io.swagger.v3.oas.annotations.media.Schema;

/**
 * 描述：仓库查询参数
 * @author mie
 * @version 1.0.0
 */
public class StoreHouseQuery {
    @Schema(description = "关键字", example = "keyword")
    private String keyword;
}
