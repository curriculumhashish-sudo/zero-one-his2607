package com.zeroone.star.dispense.entity;

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
 * 就诊（adm_encounter）
 * </p>
 *
 * @author j7
 * @since 2026-07-24
 */
@Getter
@Setter
@ToString
@TableName("adm_encounter")
public class AdmEncounter implements Serializable {

    private static final long serialVersionUID = 1L;

    /** 主键 */
    @TableId(value = "id", type = IdType.ASSIGN_ID)
    private String id;

    /** 租户标识 */
    private String tenantId;

    /** 创建时间 */
    private LocalDateTime createdAt;

    /** 最后更新时间 */
    private LocalDateTime updatedAt;

    /** 创建人 */
    private String createdBy;

    /** 最后更新人 */
    private String updatedBy;

    /** 软删标记 */
    @TableLogic
    private Boolean isDeleted;

    /** 删除时间 */
    private LocalDateTime deletedAt;

    /** 删除人 */
    private String deletedBy;

    /** 乐观锁版本号 */
    @Version
    private Integer version;

    /** 患者标识 */
    private String patientId;

    /** 分组标识 */
    private String groupId;

    /** 业务编号（如住院号） */
    private String busNo;

    /** 就诊状态枚举 */
    private String statusEnum;

    /** 就诊类别枚举（住院筛选关键字段，如 IMP=住院） */
    private String classEnum;

    /** 医保类别枚举 */
    private String ybClassEnum;

    /** 医保类别文本 */
    private String ybClassText;

    /** 就诊类别 JSON 快照 */
    private String classJson;

    /** 优先级枚举 */
    private String priorityEnum;

    /** 类型枚举 */
    private String typeEnum;

    /** 服务类型标识 */
    private String serviceTypeId;

    /** 主体状态枚举 */
    private String subjectStatusEnum;

    /** 就诊开始时间 */
    private LocalDateTime startTime;

    /** 就诊结束时间 */
    private LocalDateTime endTime;

    /** 机构标识 */
    private String organizationId;

    /** 显示顺序 */
    private Integer displayOrder;

    /** 初复诊枚举 */
    private String firstEnum;

    /** 入院来源编码 */
    private String admitSourceCode;

    /** 入院途径编码 */
    private String inWayCode;

    /** 接诊时间 */
    private LocalDateTime receptionTime;

    /** 登记人标识 */
    private String registrarId;

    /** 关联门诊就诊标识 */
    private String ambEncounterId;

    /** 出院方式编码 */
    private String outWayCode;
}
