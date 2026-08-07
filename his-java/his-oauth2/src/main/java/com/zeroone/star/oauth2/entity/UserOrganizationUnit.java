package com.zeroone.star.oauth2.entity;

import com.baomidou.mybatisplus.annotation.TableField;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

import java.io.Serializable;

/**
 * <p>
 * 用户组织单元关联表（whale_user_organization_units）
 * </p>
 *
 * @author 阿伟
 */
@Getter
@Setter
@ToString
@TableName("whale_user_organization_units")
public class UserOrganizationUnit implements Serializable {

    private static final long serialVersionUID = 1L;

    /**
     * 主键
     */
    @TableId(value = "id")
    private String id;

    /**
     * 关联用户编号（whale_user_organization_units.user_id）
     */
    @TableField("user_id")
    private String userId;

    /**
     * 组织单元编号（whale_user_organization_units.organization_unit_id）
     */
    @TableField("organization_unit_id")
    private String organizationUnitId;
}