package com.zeroone.star.project.exception.j3.register;

/**
 * <p>
 * 描述：挂号类型不存在或已删除异常
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author j3小组
 * @version 1.0.0
 */
public class RegisterNotFoundException extends RuntimeException {

    public RegisterNotFoundException(String message) {
        super(message);
    }
}
