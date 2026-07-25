package com.zeroone.star.sysmgr.exception;


import lombok.Getter;

/**
 * 系统状态枚举
 * @author 是丫丫
 * @date 2026/7/25
 */
@Getter
public enum SystemStatus {

    /**
     * 菜单不存在
     */
    MENU_NOT_FIND("菜单不存在", 4001);

    private final String message;

    private final int code;

    SystemStatus(String message, int code) {
        this.message = message;
        this.code = code;
    }

    SystemStatus(int code) {
        this.message = this.name();
        this.code = code;
    }
}
