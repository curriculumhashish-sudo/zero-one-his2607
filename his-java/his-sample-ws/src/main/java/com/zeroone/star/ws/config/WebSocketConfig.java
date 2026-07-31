package com.zeroone.star.ws.config;

import cn.hutool.json.JSONObject;
import com.zeroone.star.project.components.jwt.JwtComponent;
import jakarta.websocket.HandshakeResponse;
import jakarta.websocket.server.HandshakeRequest;
import jakarta.websocket.server.ServerEndpointConfig;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.web.socket.config.annotation.EnableWebSocket;
import org.springframework.web.socket.server.standard.ServerEndpointExporter;

import java.util.List;

@Configuration
@EnableWebSocket
public class WebSocketConfig extends ServerEndpointConfig.Configurator {

    @Bean
    public ServerEndpointExporter serverEndpointExporter(){
        return new ServerEndpointExporter();
    }

    @SuppressWarnings("all")
    @Override
    public void modifyHandshake(ServerEndpointConfig sec, HandshakeRequest request, HandshakeResponse response) {
        //从请求头获取凭证获取
        String token = request.getParameterMap().getOrDefault("token", List.of("")).get(0);
        if(token.isBlank()){
            throw new RuntimeException("token 不能为空");
        }
        sec.getUserProperties().put("uid",token);
        //通过Spring上下文获取jwt实例
        JwtComponent jwt = SpringContextHolder.getBean(JwtComponent.class);
        //解析凭证
//        try {
//            JSONObject jsonObject = new JSONObject(jwt.defaultRsaVerify(token));
//            sec.getUserProperties().put("uid",jsonObject.get("id").toString());
//        } catch (Exception e) {
//            throw new RuntimeException(e);
//        }
        super.modifyHandshake(sec, request, response);
    }
}
