package com.zeroone.star.ons.config;

import io.swagger.v3.oas.models.OpenAPI;
import io.swagger.v3.oas.models.info.Contact;
import io.swagger.v3.oas.models.info.Info;
import io.swagger.v3.oas.models.info.License;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

@Configuration
public class Knife4jConfig {

    @Bean
    public OpenAPI customOpenAPI() {
        return new OpenAPI()
                .info(new Info()
                        .title("零壹HIS - 门诊护士站接口文档")
                        .version("1.0.0-SNAPSHOT")
                        .description("门诊护士站（Outpatient Nurse Station）相关接口文档，包含医嘱执行、患者队列、执行记录等功能模块。")
                        .contact(new Contact()
                                .name("零壹星球")
                                .email("support@zeroone.star"))
                        .license(new License()
                                .name("Apache 2.0")
                                .url("https://www.apache.org/licenses/LICENSE-2.0")));
    }
}