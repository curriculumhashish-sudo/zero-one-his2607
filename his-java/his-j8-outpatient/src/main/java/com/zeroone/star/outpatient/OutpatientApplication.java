package com.zeroone.star.outpatient;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.cloud.client.discovery.EnableDiscoveryClient;

/**
 * <p>
 * 描述：门诊服务
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 01star
 * @version 1.0.0
 */
@SpringBootApplication(scanBasePackages = {
    "com.zeroone.star.outpatient",              // 本模块业务代码
    "com.zeroone.star.project.components.excel",  // Excel 组件（导出用）
    "com.zeroone.star.project.components.fastdfs" // FastDFS 组件（导出上传用，可选）
})
@EnableDiscoveryClient
public class OutpatientApplication {
    public static void main(String[] args) {
        SpringApplication.run(OutpatientApplication.class, args);
    }
}