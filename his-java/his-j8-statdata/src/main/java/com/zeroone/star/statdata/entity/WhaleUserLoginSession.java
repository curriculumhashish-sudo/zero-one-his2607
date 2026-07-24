package com.zeroone.star.statdata.entity;

import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Data;

import java.time.LocalDateTime;
/**
 * <p>描述: 用户登录会话实体</p>
 * <p>版权: &copy;01星球</p>
 * <p>地址: 01星球总部</p>
 *
 * @author highxin
 * @version 1.0.0
 */
@Data
@TableName("whale_user_login_session")
public class WhaleUserLoginSession {

    private String userId;
    private String tenantId;
    private String hospitalOrganizationId;
    private String organizationUnitId;
    private LocalDateTime updatedAt;
}