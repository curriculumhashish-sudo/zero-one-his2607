package com.zeroone.star.project.exception.j3.department;

import lombok.Getter;

@Getter
public abstract class BusinessException extends RuntimeException {
    private final int code;

    protected BusinessException(int code, String message) {
        super(message);
        this.code = code;
    }
}
