package com.zeroone.star.report.entity;

import com.baomidou.mybatisplus.annotation.TableField;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;
import java.io.Serializable;
import java.time.LocalDateTime;
import lombok.Data;

/**
 * 管理域 就诊诊断
 * @TableName adm_encounter_diagnosis
 * @author j6小组-TKORHP
 */
@TableName(value = "adm_encounter_diagnosis")
@Data
public class AdmEncounterDiagnosis implements Serializable {
    /**
     * 主键
     */
    @TableId
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
    private Integer isDeleted;

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
     * 就诊诊断关联子实体：持久化就诊与病情（Condition）的多对多事实及诊断扩展字段
     */
    private String encounterId;

    /**
     * 病情/诊断条件主键
     */
    private String conditionId;

    /**
     * 是否主诊断
     */
    private Integer primaryFlag;

    /**
     * 排序序号
     */
    private Integer rank;

    /**
     * 诊断描述
     */
    private String description;

    /**
     * 诊断类型码（术语 code 字符串，与 ipt_dise_type_code 对齐，不做整型转换）
     */
    private String diagTypeCode;

    /**
     * 诊断编码
     */
    private Integer diagCode;

    /**
     * 病情编码
     */
    private Integer conditionCode;

    /**
     * 医保或诊疗类别
     */
    private String medType;

    /**
     * 是否最高诊断
     */
    private Integer highestDiagFlag;

    /**
     * 中医证候组
     */
    private String tcmSyndromeGroup;

    /**
     * 是否中医诊断
     */
    private Integer tcmFlag;

    @TableField(exist = false)
    private static final long serialVersionUID = 1L;
}
