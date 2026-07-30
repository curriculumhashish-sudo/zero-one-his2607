package com.zeroone.star.project.exception.j3.storehouse;

import com.zeroone.star.project.enums.ResultStatus;
import com.zeroone.star.project.exception.j3.department.BusinessException;

/**
 * 库房药房不存在异常
 * @author mie
 * @version 1.0.0
 */
public class StoreHouseNotFoundException extends BusinessException {
    public StoreHouseNotFoundException(String message) {
        super(ResultStatus.FAIL.getCode(), message);
    }
}
