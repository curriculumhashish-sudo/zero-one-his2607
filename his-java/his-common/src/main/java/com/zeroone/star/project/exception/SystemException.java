package com.zeroone.star.project.exception;

/**
 * 系统管理通用业务异常，携带可自定义的业务错误码。
 *
 * @author 是丫丫
 * @date 2026/7/25
 */
public class SystemException extends RuntimeException {

    private final int code;

    public SystemException(SystemStatus status) {
        super(status.getMessage());
        this.code = status.getStatus();
    }

    public SystemException(String message, int code) {
        super(message);
        this.code = code;
    }

    public int getCode() {
        return code;
    }
}
