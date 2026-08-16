package com.zeroone.star.assistant.config;

import com.zeroone.star.project.config.filter.mcp.RequestHeaderContext;
import org.springframework.boot.web.reactive.function.client.WebClientCustomizer;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.util.StringUtils;
import org.springframework.web.reactive.function.client.ClientRequest;
import org.springframework.web.reactive.function.client.ClientResponse;
import org.springframework.web.reactive.function.client.ExchangeFunction;
import reactor.core.publisher.Mono;

import java.util.Locale;

/**
 * 将当前请求的 Authorization 头透传给 MCP 网关。
 */
@Configuration
public class McpHeaderConfig {

    @Bean
    public WebClientCustomizer assistantMcpWebClientCustomizer() {
        return builder -> builder.filter(this::addAuthorizationHeader);
    }

    private Mono<ClientResponse> addAuthorizationHeader(ClientRequest request, ExchangeFunction next) {
        String path = request.url().getPath().toLowerCase(Locale.ROOT);
        if (!path.contains("/mcp")) {
            return next.exchange(request);
        }
        String authorization = RequestHeaderContext.getAuthorization();
        if (!StringUtils.hasText(authorization)) {
            return next.exchange(request);
        }
        ClientRequest modifiedRequest = ClientRequest.from(request)
                .header("Authorization", authorization)
                .build();
        return next.exchange(modifiedRequest);
    }
}
