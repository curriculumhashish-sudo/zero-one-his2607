package com.zeroone.star.sysmgr.exception;

import com.zeroone.star.project.exception.SystemStatus;

/**
 * 角色管理状态枚举
 * <p>
 * 定义角色业务异常使用的状态码和提示信息
 * </p>
 *
 * @author j1-保温杯热水仙人
 * @since 2026-07-24
 */
public enum RoleStatus implements SystemStatus {
    /** 角色不存在 */
    ROLE_NOT_FOUND("角色不存在", 4301),
    /** 内置角色不可删除 */
    BUILT_IN_ROLE_CANNOT_DELETE("内置角色不可删除", 4302),
    /** 角色已分配给用户 */
    ROLE_ASSIGNED_TO_USER("角色已分配给用户，无法删除", 4303),
    /** 角色已被岗位使用 */
    ROLE_USED_BY_POSITION("角色已被岗位使用，无法删除", 4304),
    /** 角色创建失败 */
    ROLE_CREATE_FAILED("角色创建失败", 4305),
    /** 角色修改失败 */
    ROLE_UPDATE_FAILED("角色修改失败", 4306),
    /** 角色删除失败 */
    ROLE_DELETE_FAILED("角色删除失败", 4307);

    private final String message;
    private final int status;

    RoleStatus(String message, int status) {
        this.message = message;
        this.status = status;
    }

    @Override
    public int getStatus() {
        return status;
    }

    @Override
    public String getMessage() {
        return message;
    }
}
