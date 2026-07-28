package com.zeroone.star.configuration.entity;

import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Data;
import lombok.EqualsAndHashCode;

/**
 * <p>
 * 描述：诊疗绑定实体
 * </p>
 *
 * @author 扶舒
 * @version 1.0.0
 */
@Data
@EqualsAndHashCode(callSuper = true)
@TableName("t_treatment_binding")
public class TreatmentBinding extends BaseEntity {

    /**
     * 诊疗项目ID
     */
    private Long treatmentId;

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