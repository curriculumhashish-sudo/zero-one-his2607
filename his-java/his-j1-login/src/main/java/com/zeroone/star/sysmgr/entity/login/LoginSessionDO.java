package com.zeroone.star.sysmgr.entity.login;

import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Data;

import java.time.LocalDateTime;

/** 当前用户的租户、医院和组织单元选择状态。 */
@Data
@TableName("whale_user_login_session")
public class LoginSessionDO {
    @TableId("user_id")
    private String userId;
    private String tenantId;
    private String hospitalOrganizationId;
    private String organizationUnitId;
    private LocalDateTime updatedAt;
}
