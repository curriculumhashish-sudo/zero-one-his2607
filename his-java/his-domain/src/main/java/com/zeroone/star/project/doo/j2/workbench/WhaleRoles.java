package com.zeroone.star.project.doo.j2.workbench;

import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

import java.io.Serializable;
import java.time.LocalDateTime;

/**
 * <p>
 * 系统角色
 * </p>
 *
 * @author 鹿
 * @since 2026-07-24
 */
@Getter
@Setter
@ToString
@TableName("whale_roles")
public class WhaleRoles implements Serializable {

    private static final long serialVersionUID = 1L;

    /**
     * 主键
     */
    private String id;

    /**
     * 角色名称（唯一）
     */
    private String name;

    /**
     * 显示名称
     */
    private String displayName;

    /**
     * 角色编码
     */
    private String code;

    /**
     * 描述
     */
    private String description;

    /**
     * 是否默认角色
     */
    private Boolean isDefault;

    /**
     * 是否静态角色（不可删）
     */
    private Boolean isStatic;

    /**
     * 租户标识
     */
    private String tenantId;

    /**
     * 创建时间
     */
    private LocalDateTime createdAt;

    /**
     * 最后更新时间
     */
    private LocalDateTime updatedAt;

    /**
     * 创建人
     */
    private String createdBy;

    /**
     * 最后更新人
     */
    private String updatedBy;

    /**
     * 软删标记
     */
    private Boolean isDeleted;

    /**
     * 删除时间
     */
    private LocalDateTime deletedAt;

    /**
     * 删除人
     */
    private String deletedBy;
}
