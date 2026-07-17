package com.zeroone.star.basedata;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.cloud.client.discovery.EnableDiscoveryClient;

/**
 * <p>
 * 描述：基础数据管理模块启动入口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@SpringBootApplication
@EnableDiscoveryClient
public class BasedataApplication {

    public static void main(String[] args) {
        SpringApplication.run(BasedataApplication.class, args);
    }

}
