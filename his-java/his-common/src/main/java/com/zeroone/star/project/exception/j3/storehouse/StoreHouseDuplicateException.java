package com.zeroone.star.project.exception.j3.storehouse;

import com.zeroone.star.project.enums.ResultStatus;
import com.zeroone.star.project.exception.j3.department.BusinessException;

/**
 * 库房药房名称重复异常
 * @author mie
 * @version 1.0.0
 */
public class StoreHouseDuplicateException extends BusinessException {
    public StoreHouseDuplicateException(String message) {
        super(ResultStatus.FAIL.getCode(), message);
    }
}
