package com.zeroone.star.ws.service;

import com.zeroone.star.project.dto.notify.SampleNotifyDTO;
import com.zeroone.star.ws.config.WebSocketConfig;
import jakarta.websocket.*;
import jakarta.websocket.server.ServerEndpoint;
import org.springframework.stereotype.Component;

import java.io.IOException;
import java.util.concurrent.ConcurrentHashMap;

/**
 * <p>
 * 描述：聊天服务器
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 阿伟学长
 * @version 1.0.0
 */
@Component
@ServerEndpoint(value = "/chat", configurator = WebSocketConfig.class)
public class ChatServer {
    /**
     * 保存连接对象
     */
    private static final ConcurrentHashMap<String, Session> SESSION_POOL = new ConcurrentHashMap<>();
    private final String all = "all";

    @OnOpen
    public void onOpen(Session session) throws IOException {
        // 获取用户ID
        String uid = session.getUserProperties().get("uid").toString();
        //判断客户端对象是否存在
        if (SESSION_POOL.containsKey(uid)) {
            CloseReason reason = new CloseReason(CloseReason.CloseCodes.CANNOT_ACCEPT, "ID冲突，连接拒绝");
            session.getUserProperties().put("reason", reason);
            session.close();
            return;
        }
        //将客户端记录到Map中
        SESSION_POOL.put(uid, session);
        System.out.println("客户端（" + uid + "）：开启连接");
    }

    @OnMessage
    public String onMessage(String text, Session session) throws IOException {
        //解析消息 => ID::消息内容
        String[] msgArr = text.split("::", 2);
        //群发消息 ID=all表示群发
        if (all.equalsIgnoreCase(msgArr[0])) {
            for (Session one : SESSION_POOL.values()) {
                //排除自己
                if (session == one) {
                    continue;
                }
                //发送消息
                one.getBasicRemote().sendText(msgArr[1]);
            }
        }
        //指定发送
        else {
            Session target = SESSION_POOL.get(msgArr[0]);
            if (target != null) {
                target.getBasicRemote().sendText(msgArr[1]);
            }
        }
        return "消息发送成功";
    }

    @SuppressWarnings("resource")
    @OnClose
    public void onClose(Session session) {
        // 获取用户ID
        String uid = session.getUserProperties().get("uid").toString();
        //处理拒绝连接session关闭对象
        Object obj = session.getUserProperties().get("reason");
        if (obj instanceof CloseReason reason) {
            if (reason.getCloseCode() == CloseReason.CloseCodes.CANNOT_ACCEPT) {
                System.out.println("拒绝客户端（" + uid + "）：关闭连接");
                return;
            }
        }
        //将session对象从map中移除
        SESSION_POOL.remove(uid);
        System.out.println("客户端（" + uid + "）：关闭连接");
    }

    @OnError
    public void onError(Session session, Throwable throwable) {
        // 获取用户ID
        String uid = session.getUserProperties().get("uid").toString();
        System.out.println("客户端（" + uid + "）错误信息：" + throwable.getMessage());
    }

    /**
     * 发送通知
     * @param notify 通知对象
     */
    public void sendNotify(SampleNotifyDTO notify) {
        //群发消息 ID=all表示群发
        if (all.equalsIgnoreCase(notify.getClientId())) {
            for (Session one : SESSION_POOL.values()) {
                //发送消息
                try {
                    one.getBasicRemote().sendText(notify.getMessage());
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
        //指定发送
        else {
            Session target = SESSION_POOL.get(notify.getClientId());
            if (target != null) {
                try {
                    target.getBasicRemote().sendText(notify.getMessage());
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }
}