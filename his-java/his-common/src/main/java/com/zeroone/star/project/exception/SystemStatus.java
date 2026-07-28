package com.zeroone.star.project.exception;

/**
 * 系统状态接口，各模块业务异常枚举实现此接口统一错误码体系。
 *
 * @author 是丫丫
 * @date 2026/7/25
 */
public interface SystemStatus {

    int getStatus();

    String getMessage();
}
