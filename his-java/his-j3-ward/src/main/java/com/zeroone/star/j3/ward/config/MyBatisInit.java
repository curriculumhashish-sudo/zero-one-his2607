package com.zeroone.star.j3.ward.config;

import com.baomidou.mybatisplus.core.handlers.MetaObjectHandler;
import com.zeroone.star.project.components.user.UserDTO;
import com.zeroone.star.project.components.user.UserHolder;
import jakarta.annotation.Resource;
import lombok.extern.slf4j.Slf4j;
import org.apache.ibatis.reflection.MetaObject;
import org.mybatis.spring.annotation.MapperScan;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.Configuration;

import java.time.LocalDateTime;

@Slf4j
@Configuration
@ComponentScan("com.zeroone.star.project.config.mybatis")
@MapperScan("com.zeroone.star.j3.ward.mapper")
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
                    UserDTO currentUser = userHolder.getCurrentUser();
                    if (currentUser != null && currentUser.getUsername() != null) {
                        this.strictInsertFill(metaObject, "createdBy", String.class, currentUser.getUsername());
                    }
                } catch (Exception e) {
                    log.warn("自动填充创建人失败: {}", e.getMessage());
                }
            }
            @Override
            public void updateFill(MetaObject metaObject) {
                this.strictUpdateFill(metaObject, "updatedAt", LocalDateTime.class, LocalDateTime.now());
                try {
                    UserDTO currentUser = userHolder.getCurrentUser();
                    if (currentUser != null && currentUser.getUsername() != null) {
                        this.strictUpdateFill(metaObject, "updatedBy", String.class, currentUser.getUsername());
                    }
                } catch (Exception e) {
                    log.warn("自动填充更新人失败: {}", e.getMessage());
                }
            }
        };
    }
}
