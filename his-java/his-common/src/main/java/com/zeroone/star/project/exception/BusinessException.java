package com.zeroone.star.project.exception;

import com.zeroone.star.project.enums.ResultStatus;
import lombok.Getter;

/**
 * <p>
 * 描述：业务异常
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author zeroone
 * @version 1.0.0
 */
@Getter
public class BusinessException extends RuntimeException {

    /**
     * 状态码
     */
    private final Integer code;

    /**
     * 构造方法
     * @param message 异常消息
     */
    public BusinessException(String message) {
        super(message);
        this.code = ResultStatus.SERVER_ERROR.getCode();
    }

    /**
     * 构造方法
     * @param message 异常消息
     * @param code 状态码
     */
    public BusinessException(String message, Integer code) {
        super(message);
        this.code = code;
    }

    /**
     * 构造方法
     * @param status 状态枚举
     */
    public BusinessException(ResultStatus status) {
        super(status.getMessage());
        this.code = status.getCode();
    }
}
