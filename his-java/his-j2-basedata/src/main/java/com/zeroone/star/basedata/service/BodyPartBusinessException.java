package com.zeroone.star.basedata.service;

/**
 * 可直接反馈给调用方的部位业务校验异常。
 */
public class BodyPartBusinessException extends RuntimeException {

    public BodyPartBusinessException(String message) {
        super(message);
    }
}
