package com.zeroone.star.project.components.mybaits;

import cn.hutool.core.util.IdUtil;
import com.baomidou.mybatisplus.core.handlers.MetaObjectHandler;
import com.zeroone.star.project.components.user.UserDTO;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.components.work.WorkerIdHolder;
import lombok.RequiredArgsConstructor;
import org.apache.ibatis.reflection.MetaObject;
import org.springframework.stereotype.Component;

import java.time.LocalDateTime;

/**
 * BaseDO公共字段自动填充
 * 填充：创建时间、更新时间、创建人、更新人
 * 租户ID、软删除、版本号不在此处填充
 * @author 是丫丫
 */
@Component
@RequiredArgsConstructor
public class MybaitsHolder implements MetaObjectHandler {

    /**
     * 用户信息获取器
     */
    private final UserHolder userHolder;

    /**
     * 新增执行填充
     */
    @Override
    public void insertFill(MetaObject metaObject) {
        // 主键(雪花算法生成主键id)
        String id = String.valueOf(IdUtil.getSnowflake(WorkerIdHolder.workerId).nextId());
        this.setFieldValByNameIfNull("id", id, metaObject);
        // 时间字段
        this.setFieldValByNameIfNull("createdAt", LocalDateTime.now(), metaObject);
        this.setFieldValByNameIfNull("updatedAt", LocalDateTime.now(), metaObject);
        // TODO: 暂时写死，方便大家做测试
        // 操作人，从上下文获取登录用户ID，示例固定写死，需替换成登录上下文获取
//        UserDTO userDTO;
//        try {
//            userDTO = userHolder.getCurrentUser();
//        } catch (Exception e) {
//            throw new RuntimeException(e);
//        }
//        if (userDTO == null) {
//            throw new RuntimeException("用户信息为空");
//        }
//        String loginUserId = userDTO.getId();
//        this.setFieldValByNameIfNull("createdBy", loginUserId, metaObject);
//        this.setFieldValByNameIfNull("updatedBy", loginUserId, metaObject);
        this.setFieldValByNameIfNull("createdBy", "1", metaObject);
        this.setFieldValByNameIfNull("updatedBy", "1", metaObject);
    }

    /**
     * 修改执行填充
     */
    @Override
    public void updateFill(MetaObject metaObject) {
        this.setFieldValByNameIfNull("updatedAt", LocalDateTime.now(), metaObject);
        // TODO: 暂时写死，方便大家做测试
        // 操作人，从上下文获取登录用户ID，示例固定写死，需替换成登录上下文获取
//        UserDTO userDTO;
//        try {
//            userDTO = userHolder.getCurrentUser();
//        } catch (Exception e) {
//            throw new RuntimeException(e);
//        }
//        if (userDTO == null) {
//            throw new RuntimeException("用户信息为空");
//        }
//        String loginUserId = userDTO.getId();
//        this.setFieldValByNameIfNull("updatedBy", loginUserId, metaObject);
        this.setFieldValByNameIfNull("updatedBy", "1", metaObject);
    }

    /**
     * 当没有值的时候再设置属性，如果有值则不设置。主要是方便单元测试
     * @param fieldName 字段名
     * @param fieldVal 字段值
     * @param metaObject 元数据对象
     */
    private void setFieldValByNameIfNull(String fieldName, Object fieldVal, MetaObject metaObject) {
        // 校验实体是否存在该字段，无字段直接return，适配子类字段缺失
        if (!metaObject.hasGetter(fieldName)) {
            return;
        }
        if (metaObject.getValue(fieldName) == null) {
            this.setFieldValByName(fieldName, fieldVal, metaObject);
        }
    }
}
