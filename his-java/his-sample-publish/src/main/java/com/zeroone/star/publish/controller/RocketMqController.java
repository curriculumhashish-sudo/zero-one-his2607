package com.zeroone.star.publish.controller;

import com.zeroone.star.project.dto.notify.SampleNotifyDTO;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.publish.comp.NotifyProducer;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RestController;

/**
 * 测试消息发布控制器
 */
@RestController
@Tag(name = "rocketmq")
public class RocketMqController {
    @Resource
    NotifyProducer notifyProducer;

    @Operation(summary = "发布消息")
    @PostMapping("publish")
    public JsonVO<String> publishNotify(SampleNotifyDTO notifyDTO){
        boolean b = notifyProducer.publishNotify(notifyDTO);
        if(b){
            return JsonVO.success("发送成功");
        }
        return JsonVO.fail("发送失败");
    }
}
