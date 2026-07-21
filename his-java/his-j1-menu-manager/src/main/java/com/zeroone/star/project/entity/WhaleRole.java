package com.zeroone.star.project.entity;

import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Data;

/**
 * whale_roles 表实体
 * @author CatSN
 */
@Data
@TableName("whale_roles")
public class WhaleRole {
    @TableId
    private String id;
    private String name;
    private String displayName;
    private String code;
    private String description;
    private Integer isDefault;
    private Integer isStatic;
    private String tenantId;
    private Integer isDeleted;
}
