package com.zeroone.star.captcha.config;

import cloud.tianai.captcha.common.constant.CaptchaTypeConstant;
import cloud.tianai.captcha.resource.ResourceStore;
import cloud.tianai.captcha.resource.common.model.dto.Resource;
import cloud.tianai.captcha.spring.plugins.RedisResourceStore;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.data.redis.core.StringRedisTemplate;

/**
 * <p>
 * 描述：验证码资源配置
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 阿伟学长
 * @version 1.0.0
 */
@Configuration
public class CaptchaResourceConfiguration {
    @jakarta.annotation.Resource
    StringRedisTemplate stringRedisTemplate;

    @Bean
    public ResourceStore resourceStore() {
        // 1. 使用Redis存储
        RedisResourceStore resourceStore = new RedisResourceStore(stringRedisTemplate);
        // 2. 添加自定义背景图片
        resourceStore.addResource(CaptchaTypeConstant.SLIDER, new Resource("classpath", "bgimages/a.jpg", "default"));
        resourceStore.addResource(CaptchaTypeConstant.SLIDER, new Resource("classpath", "bgimages/b.jpg", "default"));
        resourceStore.addResource(CaptchaTypeConstant.SLIDER, new Resource("classpath", "bgimages/c.jpg", "default"));
        resourceStore.addResource(CaptchaTypeConstant.SLIDER, new Resource("classpath", "bgimages/d.jpg", "default"));
        resourceStore.addResource(CaptchaTypeConstant.SLIDER, new Resource("classpath", "bgimages/e.jpg", "default"));
        resourceStore.addResource(CaptchaTypeConstant.SLIDER, new Resource("classpath", "bgimages/g.jpg", "default"));
        resourceStore.addResource(CaptchaTypeConstant.SLIDER, new Resource("classpath", "bgimages/h.jpg", "default"));
        resourceStore.addResource(CaptchaTypeConstant.SLIDER, new Resource("classpath", "bgimages/i.jpg", "default"));
        resourceStore.addResource(CaptchaTypeConstant.SLIDER, new Resource("classpath", "bgimages/j.jpg", "default"));
        resourceStore.addResource(CaptchaTypeConstant.ROTATE, new Resource("classpath", "bgimages/48.jpg", "default"));
        resourceStore.addResource(CaptchaTypeConstant.CONCAT, new Resource("classpath", "bgimages/48.jpg", "default"));
        resourceStore.addResource(CaptchaTypeConstant.WORD_IMAGE_CLICK, new Resource("classpath", "bgimages/a.jpg", "default"));
        resourceStore.addResource(CaptchaTypeConstant.WORD_IMAGE_CLICK, new Resource("classpath", "bgimages/b.jpg", "default"));
        resourceStore.addResource(CaptchaTypeConstant.WORD_IMAGE_CLICK, new Resource("classpath", "bgimages/c.jpg", "default"));
        return resourceStore;
    }
}