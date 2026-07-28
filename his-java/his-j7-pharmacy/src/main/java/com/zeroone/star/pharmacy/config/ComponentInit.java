package com.zeroone.star.pharmacy.config;

import com.zeroone.star.project.exception.EhResultWrapper;
import com.zeroone.star.project.vo.JsonVO;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.Configuration;

/**
 * <p>
 * 描述：初始化自定义组件配置类
 * </p>
 * <p>
 * 功能说明：通过 @ComponentScan 扫描并注册 common 模块中的公共组件，包括：
 * - Swagger 配置（API 文档生成）
 * - 全局异常处理
 * - JWT 组件（Token 生成与验证）
 * - 用户信息组件（获取当前登录用户）
 * 同时注入 EhResultWrapper Bean，用于统一包装 API 响应结果。
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 01star
 * @version 1.0.0
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
