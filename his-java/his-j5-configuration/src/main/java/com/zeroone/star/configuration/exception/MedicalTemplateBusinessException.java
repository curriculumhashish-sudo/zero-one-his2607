package com.zeroone.star.configuration.exception;

/**
 * 病历模板业务异常。
 */
public class MedicalTemplateBusinessException extends RuntimeException {

    public MedicalTemplateBusinessException(String message) {
        super(message);
    }

    public MedicalTemplateBusinessException(String message, Throwable cause) {
        super(message, cause);
    }
}
