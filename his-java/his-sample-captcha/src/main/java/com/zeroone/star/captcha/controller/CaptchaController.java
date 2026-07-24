package com.zeroone.star.captcha.controller;

import cloud.tianai.captcha.application.ImageCaptchaApplication;
import cloud.tianai.captcha.application.vo.ImageCaptchaVO;
import cloud.tianai.captcha.common.constant.CaptchaTypeConstant;
import cloud.tianai.captcha.common.response.ApiResponse;
import cloud.tianai.captcha.generator.common.model.dto.GenerateParam;
import cloud.tianai.captcha.spring.plugins.secondary.SecondaryVerificationApplication;
import cloud.tianai.captcha.validator.common.model.dto.ImageCaptchaTrack;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.media.Schema;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import org.apache.commons.lang3.StringUtils;
import org.springframework.web.bind.annotation.*;

import java.util.Collections;
import java.util.concurrent.ThreadLocalRandom;

/**
 * <p>
 * 描述：验证码行为控制器，用于演示生成验证码，校验验证码
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 阿伟学长
 * @version 1.0.0
 */
@Tag(name = "CaptchaController", description = "验证码行为控制器")
@RestController
public class CaptchaController {
    @lombok.Data
    public static class Data {
        @Schema(description = "验证码id", example = "SCRAPE_9251de18b75540d39bf8ca96977a01d6", requiredMode = Schema.RequiredMode.REQUIRED)
        private String id;
        @Schema(description = "验证码数据", requiredMode = Schema.RequiredMode.REQUIRED)
        private ImageCaptchaTrack data;
    }

    @Resource
    private ImageCaptchaApplication imageCaptchaApplication;

    /**
     * 默认验证码类型为随机值
     */
    final String DEFAULT_TYPE = "RANDOM";

    /**
     * 生成验证码
     * @param type 验证码类型，不传使用随机类型
     * @return 验证码
     */
    @PostMapping("/gen")
    @Operation(summary = "生成验证码", description = "生成验证码")
    @Parameter(name = "type", description = "验证码类型，不传使用随机类型", example = "SLIDER|ROTATE|CONCAT|WORD_IMAGE_CLICK")
    public ApiResponse<ImageCaptchaVO> genCaptcha(@RequestParam(required = false) String type) {
        if (StringUtils.isBlank(type)) {
            type = DEFAULT_TYPE;
        }
        if (DEFAULT_TYPE.equals(type)) {
            int i = ThreadLocalRandom.current().nextInt(0, 4);
            type = switch (i) {
                case 0 -> CaptchaTypeConstant.SLIDER;
                case 1 -> CaptchaTypeConstant.CONCAT;
                case 2 -> CaptchaTypeConstant.ROTATE;
                default -> CaptchaTypeConstant.WORD_IMAGE_CLICK;
            };
        }
        // 生成参数
        GenerateParam generateParam = new GenerateParam();
        generateParam.setType(type);
        return imageCaptchaApplication.generateCaptcha(generateParam);
    }

    /**
     * 校验验证码
     * @param data 验证码数据
     * @return 验证结果
     */
    @PostMapping("/check")
    @Operation(summary = "校验验证码")
    public ApiResponse<?> checkCaptcha(@RequestBody Data data) {
        ApiResponse<?> response = imageCaptchaApplication.matching(data.getId(), data.getData());
        if (response.isSuccess()) {
            return ApiResponse.ofSuccess(Collections.singletonMap("id", data.getId()));
        }
        return response;
    }

    /**
     * 二次验证，一般用于机器内部调用，这里为了方便测试
     * @param id id
     * @return boolean
     */
    @GetMapping("/check2")
    @Operation(summary = "二次验证")
    public boolean check2Captcha(@RequestParam("id") String id) {
        // 如果开启了二次验证
        if (imageCaptchaApplication instanceof SecondaryVerificationApplication sa) {
            return sa.secondaryVerification(id);
        }
        return false;
    }
}