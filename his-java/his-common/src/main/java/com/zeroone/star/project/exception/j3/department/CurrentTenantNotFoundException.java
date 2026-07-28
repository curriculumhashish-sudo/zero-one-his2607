package com.zeroone.star.project.exception.j3.department;

import com.zeroone.star.project.enums.ResultStatus;

/**
 * <p>
 * 描述：科室管理业务异常类：无法获取当前用户的租户信息
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author 胡傲宇
 * @version 1.0.0
 */
public class CurrentTenantNotFoundException extends BusinessException {
    public CurrentTenantNotFoundException(String message) {
        super(ResultStatus.FAIL.getCode(), message);
    }
}
