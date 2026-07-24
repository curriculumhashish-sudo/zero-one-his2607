package com.zeroone.star.personcenter;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.cloud.client.discovery.EnableDiscoveryClient;

/**
 * <p>
 * 描述：个人中心模块
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 阿伟学长
 * @version 1.0.0
 */
//http://127.0.0.1:10600/personcenter/doc.html#/home
@SpringBootApplication
@EnableDiscoveryClient
public class PersoncenterApplication {
    public static void main(String[] args) {
        SpringApplication.run(PersoncenterApplication.class, args);
    }
}
