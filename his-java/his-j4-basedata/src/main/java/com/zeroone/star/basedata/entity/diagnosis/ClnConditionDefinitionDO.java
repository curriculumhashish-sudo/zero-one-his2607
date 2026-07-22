package com.zeroone.star.basedata.entity.diagnosis;

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
 * 描述：诊断定义实体，对应 cln_condition_definition 表
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@Getter
@Setter
@ToString
@TableName("cln_condition_definition")
public class ClnConditionDefinitionDO implements Serializable {

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
     * 来源枚举：general_dx(西医) / tcm_dx(中医) / tcm_syndrome(中医证候)
     */
    private String sourceEnum;

    /**
     * 病情编码，格式如 CDF00000001
     */
    private String conditionCode;

    /**
     * 诊断名称
     */
    private String name;

    /**
     * 拼音检索码
     */
    private String pyStr;

    /**
     * 五笔检索码
     */
    private String wbStr;

    /**
     * 诊断类型编码 → 关联字典 diag_type(309)
     */
    private String typeCode;

    /**
     * 描述说明
     */
    private String description;

    /**
     * 医保标记：0 否 / 1 是
     */
    private Boolean ybFlag;

    /**
     * 疾病编码（ICD 映射）
     */
    private String diseaseCode;

    /**
     * 医保编号
     */
    private String ybNo;

    /**
     * 医保匹配标记：0 未匹配 / 1 已匹配
     */
    private Boolean ybMatchFlag;

    /**
     * 状态枚举：ACTIVE / INACTIVE
     */
    private String statusEnum;
}
