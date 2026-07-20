package com.zeroone.star.j7.inboundapproval;

import com.zeroone.star.project.config.swagger.SwaggerConfig;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.context.annotation.Import;

/**
 * G7 入库单审批流转服务启动入口。
 *
 * @author lvv
 * @version 1.0.0
 */
@SpringBootApplication
@Import(SwaggerConfig.class)
public class InboundApprovalApplication {

    public static void main(String[] args) {
        SpringApplication.run(InboundApprovalApplication.class, args);
    }
}
