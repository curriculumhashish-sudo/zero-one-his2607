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

/**
 * <p>
 * 描述：MP初始化
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
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
                this.strictInsertFill(metaObject, "createdAt", LocalDateTime.class, LocalDateTime.now());
                try {
                    this.strictInsertFill(metaObject, "createdBy", String.class, userHolder.getCurrentUser() != null ? userHolder.getCurrentUser().getUsername() : "system");
                } catch (Exception e) {
                    throw new RuntimeException(e);
                }
            }

            @Override
            public void updateFill(MetaObject metaObject) {
                this.strictUpdateFill(metaObject, "updatedAt", LocalDateTime.class, LocalDateTime.now());
                try {
                    this.strictUpdateFill(metaObject, "updatedBy", String.class, userHolder.getCurrentUser() != null ? userHolder.getCurrentUser().getUsername() : "system");
                } catch (Exception e) {
                    throw new RuntimeException(e);
                }
            }
        };
    }

}
