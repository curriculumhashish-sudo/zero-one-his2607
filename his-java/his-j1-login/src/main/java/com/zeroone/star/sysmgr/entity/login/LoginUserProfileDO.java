package com.zeroone.star.sysmgr.entity.login;

import lombok.Data;

/** 三张登录相关表联查得到的当前用户资料。 */
@Data
public class LoginUserProfileDO {
    private String id;
    private String username;
    private String name;
    private String email;
    private String phoneNumber;
    private Byte isEnabled;
    private Boolean lockedOut;
    private String tenantId;
    private String tenantName;
    private String hospitalOrganizationId;
    private String hospitalName;
    private String organizationUnitId;
    private String organizationUnitName;
    private String practitionerId;
    private String practitionerName;
    private String practitionerStatus;
}
