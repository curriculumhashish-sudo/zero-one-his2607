package com.zeroone.star.basedata;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.cloud.client.discovery.EnableDiscoveryClient;

/**
 * <p>
 * 描述：基础数据模块
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 阿伟学长
 * @version 1.0.0
 */
//http://localhost:10300/j2-basedata/doc.html
@SpringBootApplication
@EnableDiscoveryClient
public class BasedataApplication {
    public static void main(String[] args) {
        SpringApplication.run(BasedataApplication.class, args);
    }
}
