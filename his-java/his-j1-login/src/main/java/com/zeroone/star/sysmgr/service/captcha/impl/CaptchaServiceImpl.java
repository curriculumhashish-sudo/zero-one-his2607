package com.zeroone.star.sysmgr.service.captcha.impl;
import com.zeroone.star.sysmgr.service.captcha.ICaptchaService;
import com.zeroone.star.sysmgr.entity.captcha.CaptchaDo;

import com.wf.captcha.ArithmeticCaptcha;
import jakarta.annotation.Resource;
import org.springframework.data.redis.core.StringRedisTemplate;
import org.springframework.stereotype.Service;
import org.springframework.util.StringUtils;

import java.util.UUID;
import java.util.concurrent.TimeUnit;

@Service
public class CaptchaServiceImpl implements ICaptchaService {

    @Resource
    private StringRedisTemplate stringRedisTemplate;

    private static final String PREFIX = "captcha:";
    private static final long EXPIRE = 300; // 5分钟

    @Override
    public CaptchaDo generate() {
        ArithmeticCaptcha captcha = new ArithmeticCaptcha(130, 48, 2);

        CaptchaDo result = new CaptchaDo();
        result.setUuid(UUID.randomUUID().toString().replace("-", ""));
        result.setCode(captcha.text());
        result.setImageBase64(captcha.toBase64());

        stringRedisTemplate.opsForValue().set(
                PREFIX + result.getUuid(),
                result.getCode(),
                EXPIRE,
                TimeUnit.SECONDS
        );

        return result;
    }

    @Override
    public boolean verify(String uuid, String code) {
        if (!StringUtils.hasText(uuid) || !StringUtils.hasText(code)) {
            return false;
        }
        String key = PREFIX + uuid;
        String cacheCode = stringRedisTemplate.opsForValue().get(key);
        if (cacheCode == null) return false;

        boolean result = cacheCode.equalsIgnoreCase(code.trim());
        if (result) {
            stringRedisTemplate.delete(key);
        }
        return result;
    }
}
