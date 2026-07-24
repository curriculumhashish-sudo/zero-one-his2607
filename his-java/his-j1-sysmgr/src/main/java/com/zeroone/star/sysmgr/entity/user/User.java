package com.zeroone.star.sysmgr.entity.user;

import com.baomidou.mybatisplus.annotation.TableName;
import com.baomidou.mybatisplus.annotation.IdType;
import java.time.LocalDate;
import java.time.LocalDateTime;
import com.baomidou.mybatisplus.annotation.TableId;
import java.io.Serializable;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.*;
import lombok.experimental.Accessors;

/**
 * <p>
 * 用户主表
 * </p>
 *
 * @author sykesolune
 * @since 2026-07-24
 */
@Getter
@Setter
@ToString
@TableName("whale_user")
public class User implements Serializable {

    /**
     * 主键ID
     */
    @TableId(value = "id", type = IdType.ASSIGN_ID)
    private String id;

    /**
     * 用户名
     */
    private String username;

    /**
     * 密码
     */
    private String password;

    /**
     * 用户昵称
     */
    private String nickname;

    /**
     * 手机号码
     */
    private String phoneNumber;

    /**
     * 邮箱
     */
    private String email;

    /**
     * 性别 (2:男, 1:女, 0:未知)
     */
    private Integer gender;

    /**
     * 出生日期
     */
    private LocalDate birthDate;

    /**
     * 地址
     */
    private String address;

    /**
     * 医保码
     */
    private String ybNo;

    /**
     * 签名图片URL
     */
    private String signatureImage;

    /**
     * 职称编码
     */
    private String drProfiterole;

    /**
     * 职业资格证编号
     */
    private String pharPracCertNo;

    /**
     * 科室ID
     */
    private String defaultOrgId;

    /**
     * 状态 (1:启用, 0:停用)
     */
    private Integer status;

    /**
     * 备注信息
     */
    private String remark;

    /**
     * 创建人ID
     */
    private String createBy;

    /**
     * 创建时间
     */
    private LocalDateTime createTime;

    /**
     * 更新人ID
     */
    private String updateBy;

    /**
     * 更新时间
     */
    private LocalDateTime updateTime;

    /**
     * 锁定状态(1：锁定 0：解锁)
     */
    private Integer lockStatus;


    /**
     * 登录失败次数
     */
    private Integer loginFailCount;

    /**
     * 锁定结束时间
     */
    private LocalDateTime lockEndTime;
}
