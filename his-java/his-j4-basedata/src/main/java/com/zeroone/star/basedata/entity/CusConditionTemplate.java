package com.zeroone.star.diagnosis.entity;

import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableLogic;
import com.baomidou.mybatisplus.annotation.TableName;
import com.baomidou.mybatisplus.annotation.Version;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

import java.io.Serializable;
import java.time.LocalDateTime;

/**
 * <p>
 * 描述：诊断常用模板，对应 cus_condition_template 表
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@Getter
@Setter
@ToString
@TableName("cus_condition_template")
public class CusConditionTemplate implements Serializable {

    private static final long serialVersionUID = 1L;

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
     * 软删标记：0 未删除 / 1 已删除
     */
    @TableLogic
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
    @Version
    private Integer version;

    /**
     * 模板归属主体（医生 ID / 科室 ID / 全院标识）
     */
    private String objectId;

    /**
     * 诊断定义主键 → 关联 cln_condition_definition.id
     */
    private String definitionId;

    /**
     * 绑定类型枚举：个人 / 科室 / 全院
     */
    private String bindingEnum;

    /**
     * 排序权重（越小越靠前）
     */
    private Integer sortWeight;

    /**
     * 备注
     */
    private String remark;
}
