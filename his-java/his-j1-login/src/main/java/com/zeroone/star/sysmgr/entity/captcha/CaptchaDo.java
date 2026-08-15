package com.zeroone.star.sysmgr.entity.captcha;
import lombok.Data;

@Data
public class CaptchaDo {
    private String uuid;
    private String code;
    private String imageBase64; // 临时字段，用于返回
}