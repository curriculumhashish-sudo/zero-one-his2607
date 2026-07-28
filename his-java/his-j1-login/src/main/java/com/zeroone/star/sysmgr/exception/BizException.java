package com.zeroone.star.sysmgr.exception;

import lombok.Getter;

@Getter
public class BizException extends RuntimeException {
    public BizException(String message) {
        super(message);
    }
}