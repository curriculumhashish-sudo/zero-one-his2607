package com.zeroone.star.sysmgr.exception;


/**
 * 系统状态接口
 * @author 是丫丫
 * @date 2026/7/25
 */
public interface SystemStatus {

    /**
     * 获取状态码
     * @return 状态码
     */
    public int getStatus();

    /**
     * 获取状态信息
     * @return 状态信息
     */
    public String getMessage();
}
