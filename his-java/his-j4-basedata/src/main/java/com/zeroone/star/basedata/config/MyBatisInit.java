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
                LocalDateTime now = LocalDateTime.now();
                String username = getCurrentUsername();
                // 创建时间 / 创建人
                this.strictInsertFill(metaObject, "createdAt", LocalDateTime.class, now);
                this.strictInsertFill(metaObject, "createdBy", String.class, username);
                // 插入时 lastUpdate 与 created 保持一致（组员反馈的问题）
                this.strictInsertFill(metaObject, "updatedAt", LocalDateTime.class, now);
                this.strictInsertFill(metaObject, "updatedBy", String.class, username);
            }

            @Override
            public void updateFill(MetaObject metaObject) {
                // 更新时刷新最后更新时间 / 人
                this.strictUpdateFill(metaObject, "updatedAt", LocalDateTime.class, LocalDateTime.now());
                this.strictUpdateFill(metaObject, "updatedBy", String.class, getCurrentUsername());
            }

            /**
             * 获取当前登录用户名；未登录或上下文异常时降级为 system，
             * 避免内部调用/定时任务等场景因取不到用户导致数据操作失败。
             */
            private String getCurrentUsername() {
                try {
                    return userHolder.getCurrentUser() != null
                            ? userHolder.getCurrentUser().getUsername() : "system";
                } catch (Exception e) {
                    return "system";
                }
            }
        };
    }

}
