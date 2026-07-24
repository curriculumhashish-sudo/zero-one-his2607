package com.zeroone.star.project.exception.j3.department;

import com.zeroone.star.project.enums.ResultStatus;

public class DoctorNotFoundException extends BusinessException {
    public DoctorNotFoundException(String message) {
        super(ResultStatus.FAIL.getCode(), message);
    }
}
