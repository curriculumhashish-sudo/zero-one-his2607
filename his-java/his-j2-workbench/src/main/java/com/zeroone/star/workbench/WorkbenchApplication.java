package com.zeroone.star.workbench;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.cloud.client.discovery.EnableDiscoveryClient;

/**
 * <p>
 * 描述：工作台模块
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 阿伟学长
 * @version 1.0.0
 */
//http://localhost:10500/workbench/doc.html
@SpringBootApplication
@EnableDiscoveryClient
public class WorkbenchApplication {
    public static void main(String[] args) {
        SpringApplication.run(WorkbenchApplication.class, args);
    }
}
