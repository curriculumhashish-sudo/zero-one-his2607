package com.zeroone.star.project.exception;

/**
 * 业务规则校验异常。
 *
 * <p>用于表达请求格式正确，但因数据状态、数据归属或业务规则无法完成的场景。</p>
 */
public class BusinessException extends RuntimeException {

    public BusinessException(String message) {
        super(message);
    }
}
