package com.zeroone.star.Equipment.config;

import com.zeroone.star.project.exception.EhResultWrapper;
import com.zeroone.star.project.vo.JsonVO;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.Configuration;

/**
 * <p>
 * 鎻忚堪锛氬垵濮嬪寲鑷畾涔夌粍浠? * </p>
 * <p>鐗堟潈锛?copy;01鏄熺悆</p>
 * <p>鍦板潃锛?1鏄熺悆鎬婚儴</p>
 * @author j4缁? * @version 1.0.0
 */
@Configuration
@ComponentScan({
        "com.zeroone.star.project.config.swagger",
        "com.zeroone.star.project.exception",
        "com.zeroone.star.project.components.jwt",
        "com.zeroone.star.project.components.user"
})
public class ComponentInit {

    @Bean
    public EhResultWrapper ehResultWrapper() {
        return JsonVO::create;
    }

}