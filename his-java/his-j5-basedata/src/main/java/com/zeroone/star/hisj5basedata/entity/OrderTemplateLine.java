package com.zeroone.star.hisj5basedata.entity;

import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.Getter;
import lombok.Setter;

import java.io.Serializable;
import java.math.BigDecimal;
import java.time.LocalDateTime;

/**
 * <p>
 * 组套模板明细行（映射 cus_order_template_line）
 * </p>
 * @author keepasdd
 */

@AllArgsConstructor
@Data
@TableName("cus_order_template_line")
public class OrderTemplateLine implements Serializable {

    private static final long serialVersionUID = 1L;

    /**
     * 主键
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
    private LocalDateTime createdAt;

    /**
     * 最后更新时间
     */
    private LocalDateTime updatedAt;

    /**
     * 创建人
     */
    private String createdBy;

    /**
     * 最后更新人
     */
    private String updatedBy;

    /**
     * 软删标记
     */
    private Boolean isDeleted;

    /**
     * 删除时间
     */
    private LocalDateTime deletedAt;

    /**
     * 删除人
     */
    private String deletedBy;

    /**
     * 乐观锁版本号
     */
    private Integer version;

    /**
     * 组套模板头 ID
     */
    private String orderTemplateId;

    /**
     * 行类型枚举
     */
    private String lineKindEnum;

    /**
     * 药品定义 ID
     */
    private String medicationDefinitionId;

    /**
     * 活动定义 ID
     */
    private String activityDefinitionId;

    /**
     * 设备定义 ID
     */
    private String deviceDefinitionId;

    /**
     * 分组 key
     */
    private String groupKey;

    /**
     * 排序号
     */
    private Integer sortOrder;

    /**
     * 默认数量
     */
    private BigDecimal defaultQuantity;

    /**
     * 默认单元编码
     */
    private String defaultUnitCode;

    /**
     * 默认天数
     */
    private Integer defaultDays;

    /**
     * 频次 ID
     */
    private String frequencyId;

    /**
     * 给药途径编码
     */
    private String routeCode;

    /**
     * 剂量说明
     */
    private String dosageText;

    /**
     * 默认执行科室 ID
     */
    private String defaultPerformOrgId;

    /**
     * 行备注
     */
    private String lineRemark;

    /**
     * TCM 付数
     */
    private BigDecimal chineseHerbsDoseQuantity;

    /**
     * TCM 代煎标志
     */
    private Boolean sufferingFlag;
}
