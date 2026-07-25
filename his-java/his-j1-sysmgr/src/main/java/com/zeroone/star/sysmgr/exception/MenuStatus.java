package com.zeroone.star.sysmgr.exception;

/**
 * 菜单模块异常状态枚举
 * @author Drums小白
 * @date 2026-07-25
 */
public enum MenuStatus implements SystemStatus {
    /**
     * 菜单不存在
     */
    MENU_NOT_FIND("菜单不存在", 4001),

    /**
     * 菜单创建失败
     */
    MENU_CREATE_FAIL("菜单创建失败", 4002),

    /**
     * 菜单已经存在
     */
    MENU_ALREADY_IS_EXIST("菜单已经存在", 4003),

    /**
     * 菜单状态修改失败
     */
    MENU_STATUS_EDIT_FAIL("菜单状态修改失败", 4004),

    /**
     * 菜单基础信息修改失败
     */
    MENU_EDIT_FAIL("菜单修改失败", 4005);

    private final String message;
    private final int status;

    MenuStatus(String message, int status) {
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
