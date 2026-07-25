package com.zeroone.star.project.exception.j3.department;

import lombok.Getter;

/**
 * <p>
 * 描述：科室管理业务异常抽象类
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author 胡傲宇
 * @version 1.0.0
 */
@Getter
public abstract class BusinessException extends RuntimeException {
    private final int code;

    protected BusinessException(int code, String message) {
        super(message);
        this.code = code;
    }
}
