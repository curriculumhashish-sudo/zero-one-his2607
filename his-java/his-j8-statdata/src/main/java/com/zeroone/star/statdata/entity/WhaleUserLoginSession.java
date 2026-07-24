package com.zeroone.star.statdata.entity;

import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

import java.io.Serializable;
import java.time.LocalDateTime;

/**
 * <p>
 * 用户登录后的服务端会话：租户 / 医院 / 科室选择（跨重启持久化）
 * </p>
 *
 * @author highxin
 * @since 2026-07-24
 */
@Getter
@Setter
@ToString
@TableName("whale_user_login_session")
public class WhaleUserLoginSession implements Serializable {

    private static final long serialVersionUID = 1L;

    /**
     * 用户标识
     */
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
}
