package com.zeroone.star.j3.register.config;

import com.baomidou.mybatisplus.core.handlers.MetaObjectHandler;
import com.zeroone.star.project.components.user.UserHolder;
import jakarta.annotation.Resource;
import org.apache.ibatis.reflection.MetaObject;
import org.mybatis.spring.annotation.MapperScan;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.Configuration;

import java.time.LocalDateTime;

@Configuration
// 扫描 his-common 公共配置（MyBatis-Plus + Redis 序列化等），RedisConfig 提供字符串序列化的 RedisTemplate 供 UserHolder 读取租户上下文
@ComponentScan({"com.zeroone.star.project.config.mybatis", "com.zeroone.star.project.config.redis"})
@MapperScan("com.zeroone.star.j3.register.mapper")
public class MyBatisInit {
    @Resource
    UserHolder userHolder;
    @Bean
    public MetaObjectHandler metaObjectHandler() {
        return new MetaObjectHandler() {
            @Override
            public void insertFill(MetaObject metaObject) {
                this.strictInsertFill(metaObject, "createdAt", LocalDateTime.class, LocalDateTime.now());
                try { this.strictInsertFill(metaObject, "createdBy", String.class, userHolder.getCurrentUser().getUsername()); } catch (Exception e) { System.out.println(e.getMessage()); }
            }
            @Override
            public void updateFill(MetaObject metaObject) {
                this.strictUpdateFill(metaObject, "updatedAt", LocalDateTime.class, LocalDateTime.now());
                try { this.strictUpdateFill(metaObject, "updatedBy", String.class, userHolder.getCurrentUser().getUsername()); } catch (Exception e) { System.out.println(e.getMessage()); }
            }
        };
    }
}
