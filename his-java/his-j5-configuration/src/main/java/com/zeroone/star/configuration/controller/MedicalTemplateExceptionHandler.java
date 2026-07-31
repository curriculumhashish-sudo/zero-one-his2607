package com.zeroone.star.configuration.controller;

import com.zeroone.star.configuration.exception.MedicalTemplateBusinessException;
import com.zeroone.star.project.vo.JsonVO;
import org.springframework.core.Ordered;
import org.springframework.core.annotation.Order;
import org.springframework.web.bind.annotation.ExceptionHandler;
import org.springframework.web.bind.annotation.RestControllerAdvice;

/**
 * 病历模板接口业务异常处理器。
 */
@Order(Ordered.HIGHEST_PRECEDENCE)
@RestControllerAdvice(assignableTypes = MedicalTemplateWriteController.class)
public class MedicalTemplateExceptionHandler {

    @ExceptionHandler(MedicalTemplateBusinessException.class)
    public JsonVO<Void> handleBusinessException(MedicalTemplateBusinessException exception) {
        return JsonVO.fail(exception.getMessage());
    }
}
