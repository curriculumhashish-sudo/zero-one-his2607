package com.zero.star.outpharm.config;

import io.swagger.v3.oas.models.OpenAPI;
import io.swagger.v3.oas.models.info.Contact;
import io.swagger.v3.oas.models.info.Info;
import io.swagger.v3.oas.models.info.License;
import org.springdoc.core.customizers.OpenApiCustomizer;
import org.springdoc.core.models.GroupedOpenApi;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

@Configuration
public class Knife4jConfig {

    @Bean
    public OpenAPI customOpenAPI() {
        return new OpenAPI()
                .info(new Info()
                        .title("前后端API接口文档")
                        .version("1.0.0")
                        .description("基于 Spring Boot 和 Knife4j 的门诊发药接口文档")
                        );
    }

    // 定义“患者队列”分组
    @Bean
    public GroupedOpenApi patientQueueApi() {
        return GroupedOpenApi.builder()
                .group("门诊发药-患者队列")
                .pathsToMatch("/api/j7/outpharm/patient/**")
                .build();
    }

    // 定义“药品处方”分组
    @Bean
    public GroupedOpenApi drugApi() {
        return GroupedOpenApi.builder()
                .group("门诊发药-药品处方")
                .pathsToMatch("/api/j7/outpharm/drug/**")
                .build();
    }
}