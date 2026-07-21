package com.zeroone.star.hisj5basedata;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.cloud.client.discovery.EnableDiscoveryClient;

@SpringBootApplication
@EnableDiscoveryClient
public class HisJ5BasedataApplication {

	public static void main(String[] args) {
		SpringApplication.run(HisJ5BasedataApplication.class, args);
	}

}
