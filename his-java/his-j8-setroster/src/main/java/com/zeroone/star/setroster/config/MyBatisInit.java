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
                // 新增时填充创建时间和创建人
                this.strictInsertFill(metaObject, "createdAt", LocalDateTime.class, LocalDateTime.now());
                this.strictInsertFill(metaObject, "updatedAt", LocalDateTime.class, LocalDateTime.now());
                try {
                    String username = userHolder.getCurrentUser().getUsername();
                    this.strictInsertFill(metaObject, "createdBy", String.class, username);
                    this.strictInsertFill(metaObject, "updatedBy", String.class, username);
                } catch (Exception e) {
                    // 无用户上下文时跳过，不影响主流程
                }
            }

            @Override
            public void updateFill(MetaObject metaObject) {
                // 修改时填充更新时间和更新人
                this.strictUpdateFill(metaObject, "updatedAt", LocalDateTime.class, LocalDateTime.now());
                try {
                    this.strictUpdateFill(metaObject, "updatedBy", String.class, userHolder.getCurrentUser().getUsername());
                } catch (Exception e) {
                    // 无用户上下文时跳过，不影响主流程
                }
            }
        };
    }
}
