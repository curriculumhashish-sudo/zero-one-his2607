package com.zeroone.star.j3.ward.exception;

/**
 * <p>
 * 描述：病区管理业务异常
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author zhujian
 * @version 1.0.0
 */
public class WardBusinessException extends RuntimeException {

    public WardBusinessException(String message) {
        super(message);
    }
}