package com.zeroone.star.mail.controller;

import cn.hutool.core.bean.BeanUtil;
import cn.hutool.extra.mail.Mail;
import com.zeroone.star.project.components.mail.MailComponent;
import com.zeroone.star.project.components.mail.MailMessage;
import com.zeroone.star.project.dto.mail.MailDTO;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.mail.MailVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import org.springframework.beans.BeanUtils;
import org.springframework.http.MediaType;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;
import org.springframework.web.multipart.MultipartFile;

import java.awt.*;

@RequestMapping("/mail")
@RestController
@Tag(name = "邮件发送控制器")
public class MailController {
    @Resource
    MailComponent mail;


    @Operation(summary = "发送邮件")
    @PostMapping(value = "/send",consumes = {
            MediaType.MULTIPART_FORM_DATA_VALUE,
            MediaType.APPLICATION_FORM_URLENCODED_VALUE
    })
    public JsonVO<MailVO> sendMail(MailDTO info, MultipartFile annex){
        MailMessage message = new MailMessage();
        BeanUtils.copyProperties(info,message);
        //如果有附件
        if(annex != null){
            message.setMultipartFiles(new MultipartFile[]{annex});
        }
        //发送邮件
        MailMessage sent = mail.sendMail(message);
        //结果转换成vo对象
        return JsonVO.success(BeanUtil.toBean(sent, MailVO.class));
    }
}
