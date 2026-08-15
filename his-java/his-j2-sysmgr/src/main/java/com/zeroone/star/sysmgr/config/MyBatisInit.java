package com.zeroone.star.sysmgr.config;

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
 * @author 阿伟学长
 * @version 1.0.0
 */
@Configuration
@ComponentScan("com.zeroone.star.project.config.mybatis")
@MapperScan("com.zeroone.star.sysmgr.mapper")
public class MyBatisInit {
    @Resource
    UserHolder userHolder;

    @Bean
    public MetaObjectHandler metaObjectHandler() {
        return new MetaObjectHandler() {
            @Override
            public void insertFill(MetaObject metaObject) {
                System.out.println("🔥 insertFill triggered, userHolder=" + userHolder);
                LocalDateTime now = LocalDateTime.now();
                this.strictInsertFill(metaObject, "createTime", LocalDateTime.class, now);
                this.strictInsertFill(metaObject, "createdAt", LocalDateTime.class, now);
                this.strictInsertFill(metaObject, "updatedAt", LocalDateTime.class, now);
                try {
                    var currentUser = userHolder.getCurrentUser();
                    if (currentUser != null) {
                        String username = currentUser.getUsername();
                        this.strictInsertFill(metaObject, "createdBy", String.class, username);
                        this.strictInsertFill(metaObject, "updatedBy", String.class, username);
                    } else {
                        System.out.println("⚠️ getCurrentUser() 返回 null，跳过 createdBy/updatedBy 填充");
                    }
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }

            @Override
            public void updateFill(MetaObject metaObject) {
                LocalDateTime now = LocalDateTime.now();
                this.strictUpdateFill(metaObject, "updateTime", LocalDateTime.class, now);
                this.strictUpdateFill(metaObject, "updatedAt", LocalDateTime.class, now);
                try {
                    var currentUser = userHolder.getCurrentUser();
                    if (currentUser != null) {
                        String username = currentUser.getUsername();
                        this.strictUpdateFill(metaObject, "updatedBy", String.class, username);
                    } else {
                        System.out.println("⚠️ getCurrentUser() 返回 null，跳过 updatedBy 填充");
                    }
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        };
    }
}