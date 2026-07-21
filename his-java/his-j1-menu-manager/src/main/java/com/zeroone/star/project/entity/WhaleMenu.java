package com.zeroone.star.project.entity;

import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Data;

/**
 * whale_menus 表实体
 * @author CatSN
 */
@Data
@TableName("whale_menus")
public class WhaleMenu {
    @TableId
    private String id;
    private String code;
    private String name;
    private String displayName;
    private String icon;
    private String path;
    private String component;
    private String permissionName;
    private String parentId;
    private Integer sortOrder;
    private Integer isEnabled;
    private Integer isHidden;
    private Integer isExternal;
    private String externalUrl;
    private String menuType;
    private String tenantId;
    private Integer isDeleted;
}
