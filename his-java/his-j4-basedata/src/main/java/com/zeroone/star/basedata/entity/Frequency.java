package com.zeroone.star.basedata.entity;

import com.baomidou.mybatisplus.annotation.*;
import lombok.Getter;
import lombok.Setter;
import java.io.Serializable;
import java.util.Date;

/**
 * <p>
 * 描述：频次管理实体
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@Getter
@Setter
@TableName("cus_frequency")
public class Frequency implements Serializable {

    private static final long serialVersionUID = 1L;

    /**
     * 主键 (字符串类型通常对应雪花算法等分布式ID，这里采用 ASSIGN_ID，如果是自增可改为 IdType.AUTO)
     */
    @TableId(value = "id", type = IdType.ASSIGN_ID)
    private String id;

    /**
     * 租户标识
     */
    private String tenantId;

    /**
     * 创建时间
     */
    @TableField(fill = FieldFill.INSERT)
    private Date createdAt;

    /**
     * 最后更新时间
     */
    @TableField(fill = FieldFill.INSERT_UPDATE)
    private Date updatedAt;

    /**
     * 创建人
     */
    @TableField(fill = FieldFill.INSERT)
    private String createdBy;

    /**
     * 最后更新人
     */
    @TableField(fill = FieldFill.INSERT_UPDATE)
    private String updatedBy;

    /**
     * 软删标记 (0 未删 1 已删)
     * MyBatis-Plus 逻辑删除注解，配置后 delete 方法会自动转为 update is_deleted = 1
     */
    @TableLogic(value = "0", delval = "1")
    private Byte isDeleted;

    /**
     * 删除时间
     */
    private Date deletedAt;

    /**
     * 删除人
     */
    private String deletedBy;

    /**
     * 乐观锁版本号
     * MyBatis-Plus 乐观锁注解，更新时会自动带上 version 条件并自增
     */
    @Version
    private Integer version;

    /**
     * 频次聚合根实体
     */
    private String rateCode;

    /**
     * 频次名称
     */
    private String name;

    /**
     * 周期类型
     */
    private String cycleType;

    /**
     * 日计数
     */
    private Integer dayCount;

    /**
     * 日间隔
     */
    private Integer dayInterval;

    /**
     * 日执行时点串
     */
    private String dayTimes;

    /**
     * 周周期标记
     */
    private Byte weekCycleFlag;

    /**
     * 总执行次数
     */
    private Integer totalExecutionCount;

    /**
     * 执行周期
     */
    private Integer executionPeriod;

    /**
     * 执行周期单位
     */
    private String executionPeriodUnit;

    /**
     * 适用场景
     */
    private String applicableScenes;

    /**
     * 允许跨日
     */
    private String allowCrossDay;

    /**
     * 启用标记 0 禁用 1 启用
     */
    private Byte activeFlag;

    /**
     * 排序号
     */
    private Integer sortNo;
}