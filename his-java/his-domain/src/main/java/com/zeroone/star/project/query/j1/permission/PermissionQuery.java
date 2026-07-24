package com.zeroone.star.project.query.j1.permission;

import com.zeroone.star.project.query.PageQuery;
import lombok.Data;
import lombok.EqualsAndHashCode;

@Data
@EqualsAndHashCode(callSuper = true)
public class PermissionQuery extends PageQuery {
    private String keyword;
}