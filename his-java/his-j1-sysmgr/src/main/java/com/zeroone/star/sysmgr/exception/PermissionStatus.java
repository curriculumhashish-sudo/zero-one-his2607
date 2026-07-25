package com.zeroone.star.sysmgr.exception;

/**
 * 权限管理状态枚举
 * @author j1-权限管理组
 * @date 2026/7/25
 */
public enum PermissionStatus implements SystemStatus {
    /** 权限标识已存在 */
    PERMISSION_EXISTS("权限标识已存在", 4101),
    /** 父权限不存在 */
    PARENT_NOT_EXISTS("父权限不存在", 4102),
    /** 权限不存在 */
    PERMISSION_NOT_EXISTS("权限不存在", 4103),
    /** 不能将自己设为父权限 */
    SELF_PARENT("不能将自己设为父权限", 4104),
    /** 不能将权限的子孙节点设为父权限 */
    DESCENDANT_PARENT("不能将权限的子孙节点设为父权限", 4105),
    /** 获取当前用户失败 */
    GET_USER_FAILED("获取当前用户失败", 4106);

    private final String message;
    private final int status;

    PermissionStatus(String message, int status) {
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
