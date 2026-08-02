package com.zeroone.star.project.exception.j3.department;

import com.zeroone.star.project.enums.ResultStatus;

/**
 * <p>
 * 描述：科室管理业务异常类：序列号生成失败
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author 胡傲宇
 * @version 1.0.0
 */
public class DepartmentValidationException extends BusinessException {
    public DepartmentValidationException(String message) {
        super(ResultStatus.PARAMS_INVALID.getCode(), message);
    }
}
