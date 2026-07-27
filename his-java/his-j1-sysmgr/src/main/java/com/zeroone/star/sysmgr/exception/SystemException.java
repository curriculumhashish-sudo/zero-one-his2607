package com.zeroone.star.sysmgr.exception;

/**
 * 系统管理异常
 * @author 是丫丫
 * @date 2026/7/25
 */
public class SystemException extends RuntimeException {

    private final int code;

    /**
     * 通用状态枚举异常
     * @param status 系统状态
     */
    public SystemException(SystemStatus status) {
        super(status.getMessage());
        this.code = status.getStatus();
    }

    /**
     * 自定义消息+编码异常
     * @param message 提示信息
     * @param code 异常编码
     */
    public SystemException(String message, int code) {
        super(message);
        this.code = code;
    }

    public int getCode() {
        return code;
    }
}
