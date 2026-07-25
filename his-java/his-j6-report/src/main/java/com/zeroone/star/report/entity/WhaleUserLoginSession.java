package com.zeroone.star.report.entity;

import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableField;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;
import java.io.Serializable;
import java.time.LocalDateTime;
import lombok.Data;

/**
 * 用户登录后的服务端会话：租户 / 医院 / 科室选择（跨重启持久化）
 * @TableName whale_user_login_session
 * @author wanfengzhiyunyi
 */
@TableName(value ="whale_user_login_session")
@Data
public class WhaleUserLoginSession implements Serializable {
    /**
     * 用户标识
     */
    @TableId
    private String userId;

    /**
     * 租户标识
     */
    private String tenantId;

    /**
     * hospital机构标识
     */
    private String hospitalOrganizationId;

    /**
     * 机构单元标识
     */
    private String organizationUnitId;

    /**
     * 最后更新时间
     */
    private LocalDateTime updatedAt;

    @TableField(exist = false)
    private static final long serialVersionUID = 1L;
}