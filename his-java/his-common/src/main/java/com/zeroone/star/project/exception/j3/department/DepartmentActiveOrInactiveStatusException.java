package com.zeroone.star.project.exception.j3.department;

import com.zeroone.star.project.enums.ResultStatus;

public class DepartmentActiveOrInactiveStatusException extends BusinessException {
    public DepartmentActiveOrInactiveStatusException(String message) {
        super(ResultStatus.FAIL.getCode(), message);
    }
}
