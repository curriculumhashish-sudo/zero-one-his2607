package com.zeroone.star.outpatient.entity;

import com.baomidou.mybatisplus.annotation.TableName;
import com.baomidou.mybatisplus.annotation.FieldFill;
import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableField;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableLogic;
import com.baomidou.mybatisplus.annotation.Version;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

import java.io.Serializable;
import java.time.LocalDateTime;

/**
 * <p>
 * 系统用户
 * </p>
 *
 * @author Zzen
 * @since 2026-07-23
 */
@Getter
@Setter
@ToString
@TableName("whale_users")
public class WhaleUsers implements Serializable {

    private static final long serialVersionUID = 1L;

    /**
     * 主键
     */
    @TableId(type = IdType.ASSIGN_UUID)
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
    @TableField(fill = FieldFill.INSERT)
    private LocalDateTime createdAt;

    /**
     * 最后更新时间
     */
    @TableField(fill = FieldFill.INSERT_UPDATE)
    private LocalDateTime updatedAt;

    /**
     * 创建人
     */
    @TableField(fill = FieldFill.INSERT)
    private String createdBy;

    /**
     * 最后更新人
     */
    @TableField(fill = FieldFill.INSERT_UPDATE)
    private String updatedBy;

    /**
     * 软删标记
     */
    @TableLogic
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
