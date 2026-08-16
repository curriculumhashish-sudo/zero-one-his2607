package com.zeroone.star.project.dto.mail;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * <p>
 * 描述：书写一个邮件发送上传数据模型
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 阿伟学长
 * @version 1.0.0
 */
@Data
@Schema(description = "邮件发送上传数据对象")
public class MailDTO {
    @Schema(description = "邮件接收人（多个邮箱则用逗号\",\"隔开）", example = "xxx@qq.com,李明<xxxx@163.com>", requiredMode = Schema.RequiredMode.REQUIRED)
    private String to;
    @Schema(description = "抄送（多个邮箱则用逗号\",\"隔开）", example = "xxx@qq.com,李明<xxxx@163.com>")
    private String cc;
    @Schema(description = "密送（多个邮箱则用逗号\",\"隔开）", example = "xxx@qq.com,李明<xxxx@163.com>")
    private String bcc;
    @Schema(description = "邮件主题", example = "测试邮件主题", requiredMode = Schema.RequiredMode.REQUIRED)
    private String subject;
    @Schema(description = "邮件内容", example = "测试邮件内容", requiredMode = Schema.RequiredMode.REQUIRED)
    private String text;
}