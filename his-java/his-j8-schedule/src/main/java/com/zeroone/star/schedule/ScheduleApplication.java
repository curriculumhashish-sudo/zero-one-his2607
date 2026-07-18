package com.zeroone.star.schedule;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.cloud.client.discovery.EnableDiscoveryClient;

/**
 * <p>
 * Description: Schedule Service Application
 * </p>
 * <p>Copyright: copy;01star</p>
 * <p>Address: 01star Headquarters</p>
 * @author zeroone
 * @version 1.0.0
 */
@SpringBootApplication
@EnableDiscoveryClient
public class ScheduleApplication {
    public static void main(String[] args) {
        SpringApplication.run(ScheduleApplication.class, args);
    }
}