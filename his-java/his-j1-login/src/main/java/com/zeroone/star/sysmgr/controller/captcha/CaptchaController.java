package com.zeroone.star.sysmgr.controller.captcha;


import com.zeroone.star.project.dto.j1.login.CaptchaVerifyDTO;
import com.zeroone.star.project.j1.login.ICaptchaApi;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j1.login.CaptchaVO;
import com.zeroone.star.sysmgr.entity.captcha.CaptchaDo;
import com.zeroone.star.sysmgr.service.captcha.ICaptchaService;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import jakarta.validation.Valid;
import org.springframework.web.bind.annotation.*;

@Tag(name = "j1-登录-验证码")
@RestController
@RequestMapping("/auth/captcha")
public class CaptchaController implements ICaptchaApi {

    @Resource
    private ICaptchaService captchaService;

    @Override
    @PostMapping("/generate")
    public JsonVO<CaptchaVO> generate() {
        CaptchaDo captchado = captchaService.generate();

        CaptchaVO vo = new CaptchaVO();
        vo.setUuid(captchado.getUuid());
        vo.setImageBase64(captchado.getImageBase64());
        return JsonVO.success(vo);
    }

    @Override
    @PostMapping("/verify")
    public JsonVO<Boolean> verify(@Valid @RequestBody CaptchaVerifyDTO dto) {
        boolean result = captchaService.verify(dto.getUuid(), dto.getCode());
        return JsonVO.success(result);
    }
}
