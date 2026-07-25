package com.zeroone.star.j3.patient.exception;

import com.zeroone.star.j3.patient.controller.PatientController;
import com.zeroone.star.project.vo.JsonVO;
import org.springframework.core.Ordered;
import org.springframework.core.annotation.Order;
import org.springframework.web.bind.annotation.ExceptionHandler;
import org.springframework.web.bind.annotation.RestControllerAdvice;

/**
 * 患者管理业务异常响应处理。
 *
 * @author 照夜清
 */
@Order(Ordered.HIGHEST_PRECEDENCE)
@RestControllerAdvice(assignableTypes = PatientController.class)
public class PatientExceptionHandler {

    @ExceptionHandler(PatientBusinessException.class)
    public JsonVO<Void> handlePatientBusinessException(
            PatientBusinessException exception
    ) {
        return JsonVO.fail(exception.getMessage());
    }
}
