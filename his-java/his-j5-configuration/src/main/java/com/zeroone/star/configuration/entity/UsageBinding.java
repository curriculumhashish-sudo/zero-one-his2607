package com.zeroone.star.configuration.entity;

import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Data;
import lombok.EqualsAndHashCode;

import java.time.LocalTime;

/**
 * <p>
 * 描述：用法绑定实体（映射cus_service_location_template表）
 * </p>
 *
 * @author 扶舒
 * @version 1.0.0
 */
@Data
@EqualsAndHashCode(callSuper = true)
@TableName("cus_service_location_template")
public class UsageBinding extends BaseEntity {

    /**
     * 机构ID
     */
    private String organizationId;

    /**
     * 项目表（如：healthcare_service）
     */
    private String itemTable;

    /**
     * 分配分类编码
     */
    private String distributionCategoryCode;

    /**
     * 默认执行地点ID
     */
    private String defLocationId;

    /**
     * 项目定义ID
     */
    private String itemDefinitionId;

    /**
     * 方法编码
     */
    private String methodCode;

    /**
     * 开始时间
     */
    private LocalTime startTime;

    /**
     * 结束时间
     */
    private LocalTime endTime;

    /**
     * 优先级
     */
    private Integer priority;

    /**
     * 显示顺序
     */
    private Integer displayOrder;

    /**
     * 备注
     */
    private String remark;
}
