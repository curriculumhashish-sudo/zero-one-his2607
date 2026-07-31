package com.zeroone.star.project.vo.mail;

import com.fasterxml.jackson.annotation.JsonFormat;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import org.springframework.format.annotation.DateTimeFormat;

import java.util.Date;

/**
 * <p>
 * 描述：书写一个邮件发送结果显示数据模型
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 阿伟学长
 * @version 1.0.0
 */
@Data
@Schema(description = "邮件发送结果显示数据对象")
public class MailVO {
    @Schema(description = "发送时间", example = "2024-11-21", requiredMode = Schema.RequiredMode.REQUIRED)
    @DateTimeFormat(pattern = "yyyy-MM-dd HH:mm:ssS")
    @JsonFormat(pattern = "yyyy-MM-dd HH:mm:ssS")
    private Date sentDate;
    @Schema(description = "状态", example = "ok或fail", requiredMode = Schema.RequiredMode.REQUIRED)
    private String status;
    @Schema(description = "错误信息", example = "错误信息")
    private String error;
}
