package com.zeroone.star.Treatment;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.cloud.client.discovery.EnableDiscoveryClient;

/**
 * <p>
 * 鎻忚堪锛?{d}妯″潡鍚姩鍏ュ彛
 * </p>
 * <p>鐗堟潈锛?copy;01鏄熺悆</p>
 * <p>鍦板潃锛?1鏄熺悆鎬婚儴</p>
 * @author j4缁? * @version 1.0.0
 */
@SpringBootApplication
@EnableDiscoveryClient
public class TreatmentApplication {

    public static void main(String[] args) {
        SpringApplication.run(TreatmentApplication.class, args);
    }

}