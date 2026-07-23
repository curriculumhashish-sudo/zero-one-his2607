package com.zeroone.star.j7.dispense;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
<<<<<<< HEAD
import org.springframework.boot.autoconfigure.jdbc.DataSourceAutoConfiguration;
=======
import org.springframework.cloud.client.discovery.EnableDiscoveryClient;

>>>>>>> 513c733e37f8c86f6581e6d4b1be4958cbc2e84b
import org.springframework.cloud.client.discovery.EnableDiscoveryClient;

@SpringBootApplication
@EnableDiscoveryClient
public class DispenseApplication {
    public static void main(String[] args) {
        SpringApplication.run(DispenseApplication.class, args);
    }
}
