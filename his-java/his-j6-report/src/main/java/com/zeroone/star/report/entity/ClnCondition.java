package com.zeroone.star.report.entity;

import com.baomidou.mybatisplus.annotation.TableField;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;
import java.io.Serializable;
import java.time.LocalDateTime;
import lombok.Data;

/**
 * 临床 病情
 * @TableName cln_condition
 * @author j6小组-TKORHP
 */
@TableName(value = "cln_condition")
@Data
public class ClnCondition implements Serializable {
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
     * verification状态枚举值
     */
    private String verificationStatusEnum;

    /**
     * clinical状态枚举值
     */
    private String clinicalStatusEnum;

    /**
     * category枚举值
     */
    private String categoryEnum;

    /**
     * severity枚举值
     */
    private String severityEnum;

    /**
     * 业务编号
     */
    private String busNo;

    /**
     * 身体site JSON
     */
    private String bodySiteJson;

    /**
     * 身体结构标识
     */
    private String bodyStructureId;

    /**
     * 患者标识
     */
    private String patientId;

    /**
     * 描述
     */
    private String description;

    /**
     * 发病时间
     */
    private LocalDateTime onsetDatetime;

    /**
     * 发病年龄
     */
    private Integer onsetAge;

    /**
     * onset文本
     */
    private String onsetText;

    /**
     * 缓解时间
     */
    private LocalDateTime abatementDatetime;

    /**
     * 缓解年龄
     */
    private Integer abatementAge;

    /**
     * 缓解描述
     */
    private String abatementString;

    /**
     * 记录 datetime
     */
    private LocalDateTime recordedDatetime;

    /**
     * 记录人标识
     */
    private String recorderId;

    /**
     * stage summary 文本
     */
    private String stageSummaryText;

    /**
     * 临床证据
     */
    private String evidence;

    /**
     * 医保编号
     */
    private String ybNo;

    /**
     * 定义标识
     */
    private String definitionId;

    /**
     * tcm标志位
     */
    private Integer tcmFlag;

    /**
     * 诊断记录聚合根
     */
    private String encounterId;

    @TableField(exist = false)
    private static final long serialVersionUID = 1L;
}
