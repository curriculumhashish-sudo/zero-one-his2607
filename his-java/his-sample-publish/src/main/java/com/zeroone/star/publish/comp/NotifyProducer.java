package com.zeroone.star.publish.comp;

import com.zeroone.star.project.dto.notify.SampleNotifyDTO;
import jakarta.annotation.Resource;
import javassist.tools.rmi.Sample;
import org.springframework.cloud.stream.function.StreamBridge;
import org.springframework.messaging.support.GenericMessage;
import org.springframework.stereotype.Component;

/**
 * 通知消息生产者
 */
@Component
public class NotifyProducer {
    @Resource
    private StreamBridge streamBridge;

    /**
     * 发送通知消息
     * @param notifyDTO 消息对象
     * @return 发送结果
     */
    public boolean publishNotify(SampleNotifyDTO notifyDTO){
        GenericMessage<SampleNotifyDTO> message = new GenericMessage(notifyDTO);
        boolean sent = streamBridge.send("producer-out-0", message);
        return sent;
    }
}
