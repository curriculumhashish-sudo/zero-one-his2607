package com.zeroone.star.configuration.entity;

import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Data;
import lombok.EqualsAndHashCode;

/**
 * <p>
 * 描述：用法绑定实体
 * </p>
 *
 * @author 扶舒
 * @version 1.0.0
 */
@Data
@EqualsAndHashCode(callSuper = true)
@TableName("t_usage_binding")
public class UsageBinding extends BaseEntity {

    /**
     * 用法ID
     */
    private Long usageId;

    /**
     * 绑定目标类型：1=诊疗，2=耗材
     */
    private Integer targetType;

    /**
     * 绑定目标名称
     */
    private String targetName;

    /**
     * 数量
     */
    private Integer quantity;

    /**
     * 单位
     */
    private String unit;

    /**
     * 状态：1启用，0停用
     */
    private Integer status;
}