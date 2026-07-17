package com.zeroone.star.j3.department;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.cloud.client.discovery.EnableDiscoveryClient;

/**
 * <p>
 * 描述：科室管理模块
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 很帅大王
 * @version 1.0.0
 */
@SpringBootApplication
@EnableDiscoveryClient
public class DepartmentApplication {
    public static void main(String[] args) {
        SpringApplication.run(DepartmentApplication.class, args);
    }
}
