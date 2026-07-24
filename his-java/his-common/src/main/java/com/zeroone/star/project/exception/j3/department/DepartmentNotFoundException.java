package com.zeroone.star.project.exception.j3.department;

import com.zeroone.star.project.enums.ResultStatus;

public class DepartmentNotFoundException extends BusinessException {
    public DepartmentNotFoundException(String message) {
        super(ResultStatus.FAIL.getCode(), message);
    }
}
