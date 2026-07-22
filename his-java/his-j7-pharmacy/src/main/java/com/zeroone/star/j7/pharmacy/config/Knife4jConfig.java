package com.zeroone.star.j7.pharmacy.config;

import org.springdoc.core.models.GroupedOpenApi;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;


@Configuration
public class Knife4jConfig {


    @Bean
    public GroupedOpenApi purchaseInboundApi() {
        return GroupedOpenApi.builder()
                .group("药房管理-采购入库")
                .pathsToMatch("/api/j7/pharmacy/purchaseInbound/**")
                .build();
    }
}