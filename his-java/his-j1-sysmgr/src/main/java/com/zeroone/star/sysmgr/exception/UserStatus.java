package com.zeroone.star.sysmgr.exception;

public enum UserStatus implements SystemStatus{

    USER_NOT_FIND("用户不存在", 4201),
    GET_USER_FAILED("获取当前用户失败", 4202);


    private final String message;
    private final int status;

    UserStatus(String message, int status) {
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
