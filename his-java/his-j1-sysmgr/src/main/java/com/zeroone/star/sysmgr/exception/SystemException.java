package com.zeroone.star.sysmgr.exception;


/**
 * 系统管理异常
 * @author 是丫丫
 * @date 2026/7/25
 */
public class SystemException extends RuntimeException {

    public SystemException(SystemStatus status) {
        super(status.getMessage());
    }
}
