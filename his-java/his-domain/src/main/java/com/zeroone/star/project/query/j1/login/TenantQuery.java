package com.zeroone.star.project.query.j1.login;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import lombok.EqualsAndHashCode;

@Data
@EqualsAndHashCode(callSuper = true)
@Schema(description = "租户查询条件")
public class TenantQuery extends PageQuery {

    @Schema(description = "用户ID（从登录态获取，可不传）", example = "1001")
    private Long userId;
}