package com.zeroone.star.project.exception.j3.storehouse;

import com.zeroone.star.project.enums.ResultStatus;
import com.zeroone.star.project.exception.j3.department.BusinessException;

/**
 * 库房药房租户越权异常
 * @author mie
 * @version 1.0.0
 */
public class StoreHouseTenantAccessException extends BusinessException {
    public StoreHouseTenantAccessException(String message) {
        super(ResultStatus.FAIL.getCode(), message);
    }
}
