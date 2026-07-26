package com.zeroone.star.outpatient.entity;

import com.baomidou.mybatisplus.annotation.TableName;
import com.baomidou.mybatisplus.annotation.FieldFill;
import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableField;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableLogic;
import com.baomidou.mybatisplus.annotation.Version;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

import java.io.Serializable;
import java.time.LocalDateTime;

/**
 * <p>
 * 管理域 就诊
 * </p>
 *
 * @author Zzen
 * @since 2026-07-23
 */
@Getter
@Setter
@ToString
@TableName("adm_encounter")
public class AdmEncounter implements Serializable {

    private static final long serialVersionUID = 1L;

    /**
     * 主键
     */
    @TableId(type = IdType.ASSIGN_UUID)
    private String id;

    /**
     * 租户标识
     */
    private String tenantId;

    /**
     * 创建时间
     */
    @TableField(fill = FieldFill.INSERT)
    private LocalDateTime createdAt;

    /**
     * 最后更新时间
     */
    @TableField(fill = FieldFill.INSERT_UPDATE)
    private LocalDateTime updatedAt;

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
     * 软删标记
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
     * 就诊聚合根实体
     */
    private String patientId;

    /**
     * group 标识
     */
    private String groupId;

    /**
     * bus 编号
     */
    private String busNo;

    /**
     * status 枚举值
     */
    private String statusEnum;

    /**
     * class 枚举值
     */
    private String classEnum;

    /**
     * ybClass 枚举值
     */
    private String ybClassEnum;

    /**
     * 医保class文本
     */
    private String ybClassText;

    /**
     * class JSON 快照
     */
    private String classJson;

    /**
     * priority 枚举值
     */
    private String priorityEnum;

    /**
     * type 枚举值
     */
    private String typeEnum;

    /**
     * serviceType 标识
     */
    private String serviceTypeId;

    /**
     * subjectStatus 枚举值
     */
    private String subjectStatusEnum;

    /**
     * start 时间
     */
    private LocalDateTime startTime;

    /**
     * end 时间
     */
    private LocalDateTime endTime;

    /**
     * 机构 标识
     */
    private String organizationId;

    /**
     * 显示顺序
     */
    private Integer displayOrder;

    /**
     * first 枚举值
     */
    private String firstEnum;

    /**
     * admitSource 代码
     */
    private String admitSourceCode;

    /**
     * inWay 代码
     */
    private String inWayCode;

    /**
     * reception 时间
     */
    private LocalDateTime receptionTime;

    /**
     * registrar 标识
     */
    private String registrarId;

    /**
     * ambEncounter 标识
     */
    private String ambEncounterId;

    /**
     * outWay 代码
     */
    private String outWayCode;
}
