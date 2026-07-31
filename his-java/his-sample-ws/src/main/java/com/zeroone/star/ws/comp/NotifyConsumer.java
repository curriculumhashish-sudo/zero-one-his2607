package com.zeroone.star.ws.comp;

import com.zeroone.star.project.dto.notify.SampleNotifyDTO;
import com.zeroone.star.ws.service.ChatServer;
import jakarta.annotation.Resource;
import lombok.extern.slf4j.Slf4j;
import org.springframework.messaging.Message;
import org.springframework.stereotype.Component;

import java.util.function.Consumer;


@Component
@Slf4j
public class NotifyConsumer {

    @Resource
    ChatServer chatServer;

    public Consumer<Message<SampleNotifyDTO>> consumer(){
        return message -> {
            SampleNotifyDTO notify  = message.getPayload();
            log.info("接收到的消息是："+ notify);
            chatServer.sendNotify(message.getPayload());
        };
    }

}
