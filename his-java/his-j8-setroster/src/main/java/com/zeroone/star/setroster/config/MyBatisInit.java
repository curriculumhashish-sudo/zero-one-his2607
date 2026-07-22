package com.zeroone.star.setroster.config;

import com.baomidou.mybatisplus.core.handlers.MetaObjectHandler;
import com.zeroone.star.project.components.user.UserHolder;
import jakarta.annotation.Resource;
import org.apache.ibatis.reflection.MetaObject;
import org.mybatis.spring.annotation.MapperScan;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.Configuration;

import java.time.LocalDateTime;

/**
 * <p>
 * 描述：MP初始化
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 01star
 * @version 1.0.0
 */
@Configuration
@ComponentScan("com.zeroone.star.project.config.mybatis")
@MapperScan("com.zeroone.star.setroster.mapper")
public class MyBatisInit {
    @Resource
    UserHolder userHolder;

    @Bean
    public MetaObjectHandler metaObjectHandler() {
        return new MetaObjectHandler() {
            @Override
            public void insertFill(MetaObject metaObject) {
                LocalDateTime now = LocalDateTime.now();
                String userId = currentUserId();
                this.strictInsertFill(metaObject, "createdAt", LocalDateTime.class, now);
                this.strictInsertFill(metaObject, "updatedAt", LocalDateTime.class, now);
                this.strictInsertFill(metaObject, "createdBy", String.class, userId);
                this.strictInsertFill(metaObject, "updatedBy", String.class, userId);
            }

            @Override
            public void updateFill(MetaObject metaObject) {
                this.setFieldValByName("updatedAt", LocalDateTime.now(), metaObject);
                this.setFieldValByName("updatedBy", currentUserId(), metaObject);
            }

            private String currentUserId() {
                try {
                    String userId = userHolder.getCurrentUser().getId();
                    return userId == null || userId.isBlank() ? "system" : userId;
                } catch (Exception ignored) {
                    return "system";
                }
            }
        };
    }
}
