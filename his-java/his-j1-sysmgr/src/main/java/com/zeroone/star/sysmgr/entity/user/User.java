package com.zeroone.star.sysmgr.entity.user;

import com.baomidou.mybatisplus.annotation.TableName;

import java.time.LocalDateTime;

import com.zeroone.star.project.entity.BaseDO;
import lombok.*;

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
    private Integer isActive;

    /**
     * 是否锁定
     */
    private Integer isLockedOut;

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
