package com.zeroone.star.j3.patient.config;

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
@ComponentScan("com.zeroone.star.project.config.mybatis")
@MapperScan("com.zeroone.star.j3.patient.mapper")
public class MyBatisInit {
    @Resource
    UserHolder userHolder;

    @Bean
    public MetaObjectHandler metaObjectHandler() {
        return new MetaObjectHandler() {
            @Override
            public void insertFill(MetaObject metaObject) {
                this.strictInsertFill(metaObject, "createdAt", LocalDateTime.class, LocalDateTime.now());
                try {
                    this.strictInsertFill(metaObject, "createdBy", String.class, userHolder.getCurrentUser().getId());
                } catch (Exception e) {
                    System.out.println(e.getMessage());
                }
            }

            @Override
            public void updateFill(MetaObject metaObject) {
                this.strictUpdateFill(metaObject, "updatedAt", LocalDateTime.class, LocalDateTime.now());
                try {
                    this.strictUpdateFill(metaObject, "updatedBy", String.class, userHolder.getCurrentUser().getId());
                } catch (Exception e) {
                    System.out.println(e.getMessage());
                }
            }
        };
    }
}
