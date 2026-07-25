package com.zeroone.star.sysmgr.exception;

/**
 * 菜单状态枚举
 * @author 是丫丫
 * @date 2026/7/25
 */
public enum MenuStatus implements SystemStatus {
    /**
     * 菜单不存在
     */
    MENU_NOT_FIND("菜单不存在", 4001),

    /**
     * 菜单创建失败
     */
    MENU_CREATE_FAIL("菜单创建失败", 500),

    /**
     * 菜单状态修改失败
     */
    MENU_STATUS_EDIT_FAIL("菜单状态修改失败", 4003);

    private final String message;

    private final int status;

    MenuStatus(String message, int code) {
        this.message = message;
        this.status = code;
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
