package com.zeroone.star.basedata.config;

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
@MapperScan("com.zeroone.star.basedata.mapper")
public class MyBatisInit {

    @Resource
    UserHolder userHolder;

    @Bean
    public MetaObjectHandler metaObjectHandler() {
        return new MetaObjectHandler() {
            @Override
            public void insertFill(MetaObject metaObject) {
                this.strictInsertFill(metaObject, "createTime", LocalDateTime.class, LocalDateTime.now());
                this.strictInsertFill(metaObject, "createdAt", LocalDateTime.class, LocalDateTime.now());
                this.strictInsertFill(metaObject, "updatedAt", LocalDateTime.class, LocalDateTime.now());
                this.strictInsertFill(metaObject, "isDeleted", Integer.class, 0);
                try {
                    String username = userHolder.getCurrentUser() != null ? userHolder.getCurrentUser().getUsername() : "system";
                    this.strictInsertFill(metaObject, "createBy", String.class, username);
                    this.strictInsertFill(metaObject, "createdBy", String.class, username);
                } catch (Exception e) {
                    throw new RuntimeException(e);
                }
            }

            @Override
            public void updateFill(MetaObject metaObject) {
                this.strictUpdateFill(metaObject, "updateTime", LocalDateTime.class, LocalDateTime.now());
                this.strictUpdateFill(metaObject, "updatedAt", LocalDateTime.class, LocalDateTime.now());
                try {
                    String username = userHolder.getCurrentUser() != null ? userHolder.getCurrentUser().getUsername() : "system";
                    this.strictUpdateFill(metaObject, "updateBy", String.class, username);
                    this.strictUpdateFill(metaObject, "updatedBy", String.class, username);
                } catch (Exception e) {
                    throw new RuntimeException(e);
                }
            }
        };
    }

}