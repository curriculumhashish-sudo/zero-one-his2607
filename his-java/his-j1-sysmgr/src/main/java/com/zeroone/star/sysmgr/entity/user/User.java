package com.zeroone.star.sysmgr.entity.user;

import com.baomidou.mybatisplus.annotation.TableName;

import java.time.LocalDateTime;
import java.io.Serializable;

import com.zeroone.star.project.entity.BaseDO;
import lombok.*;
import lombok.experimental.Accessors;

/**
 * <p>
 * 系统用户
 * </p>
 *
 * @author sykesolune
 * @since 2026-07-24
 */
@Getter
@Setter
@ToString
@TableName("whale_users")
public class User extends BaseDO {
    /**
     * 主键
     */
    private String id;

    /**
     * 登录用户名
     */
    private String userName;

    /**
     * 邮箱
     */
    private String email;

    /**
     * 手机号
     */
    private String phoneNumber;

    /**
     * 密码哈希
     */
    private String passwordHash;

    /**
     * 名
     */
    private String name;

    /**
     * 姓
     */
    private String surname;

    /**
     * 是否启用
     */
    private Boolean isActive;

    /**
     * 是否锁定
     */
    private Boolean isLockedOut;

    /**
     * 锁定结束时间
     */
    private LocalDateTime lockoutEnd;

    /**
     * 登录失败次数
     */
    private Integer accessFailedCount;

    /**
     * 扩展属性 JSON
     */
    private String extraProperties;

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

    /**
     * UKey 标识
     */
    private String ukeyId;

    /**
     * UKey 来源标识
     */
    private String ukeySrcId;


}
