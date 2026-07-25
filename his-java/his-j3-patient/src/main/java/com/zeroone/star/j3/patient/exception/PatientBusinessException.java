package com.zeroone.star.j3.patient.exception;

/**
 * 患者管理业务异常。
 *
 * @author 照夜清
 */
public class PatientBusinessException extends RuntimeException {

    public PatientBusinessException(String message) {
        super(message);
    }
}
