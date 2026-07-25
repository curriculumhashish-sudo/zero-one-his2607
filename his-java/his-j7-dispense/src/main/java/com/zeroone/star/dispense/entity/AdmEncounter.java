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

    @TableId(value = "id", type = IdType.ASSIGN_ID)
    private String id;

    private String tenantId;

    private LocalDateTime createdAt;

    private LocalDateTime updatedAt;

    private String createdBy;

    private String updatedBy;

    @TableLogic
    private Boolean isDeleted;

    private LocalDateTime deletedAt;

    private String deletedBy;

    @Version
    private Integer version;

    private String patientId;

    private String groupId;

    /** 住院号等业务编号 */
    private String busNo;

    private String statusEnum;

    /** 就诊类别（住院筛选关键字段） */
    private String classEnum;

    private String ybClassEnum;

    private String ybClassText;

    private String classJson;

    private String priorityEnum;

    private String typeEnum;

    private String serviceTypeId;

    private String subjectStatusEnum;

    private LocalDateTime startTime;

    private LocalDateTime endTime;

    private String organizationId;

    private Integer displayOrder;

    private String firstEnum;

    private String admitSourceCode;

    private String inWayCode;

    private LocalDateTime receptionTime;

    private String registrarId;

    private String ambEncounterId;

    private String outWayCode;
}
