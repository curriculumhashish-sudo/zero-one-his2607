package com.zeroone.star.sysmgr.entity.login;

import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Data;

import java.io.Serializable;

/**
 * 登录认证用户数据对象，对应 whale_users 表。
 */
@Data
@TableName("whale_users")
public class LoginUserDO implements Serializable {

    private String id;

    private String userName;
    private String email;
    private String phoneNumber;
    private String name;
    private Byte isActive;
    private Byte isLockedOut;
    private Byte isDeleted;
}
