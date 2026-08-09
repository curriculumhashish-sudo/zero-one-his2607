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
                LocalDateTime now = LocalDateTime.now();
                this.strictInsertFill(metaObject, "createTime", LocalDateTime.class, now);
                this.strictInsertFill(metaObject, "createdAt", LocalDateTime.class, now);  // 新增
                this.strictInsertFill(metaObject, "updatedAt", LocalDateTime.class, now);  // 新增
                try {
                    this.strictInsertFill(metaObject, "createBy", String.class, userHolder.getCurrentUser().getUsername());
                } catch (Exception e) {
                    System.out.println(e.getMessage());
                }
            }

            @Override
            public void updateFill(MetaObject metaObject) {
                LocalDateTime now = LocalDateTime.now();
                this.strictUpdateFill(metaObject, "updateTime", LocalDateTime.class, now);
                this.strictUpdateFill(metaObject, "updatedAt", LocalDateTime.class, now);  // 新增
                try {
                    this.strictUpdateFill(metaObject, "updateBy", String.class, userHolder.getCurrentUser().getUsername());
                } catch (Exception e) {
                    System.out.println(e.getMessage());
                }
            }
        };
    }
}
