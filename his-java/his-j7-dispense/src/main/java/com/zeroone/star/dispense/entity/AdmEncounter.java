package com.zeroone.star.dispense.entity;

import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Getter;
import lombok.Setter;

import java.io.Serializable;
import java.time.LocalDateTime;

@Getter
@Setter
@TableName("adm_encounter")
public class AdmEncounter implements Serializable {
    private static final long serialVersionUID = 1L;
    @TableId
    private String id;
    private String tenantId;
    private LocalDateTime createdAt;
    private LocalDateTime updatedAt;
    private String createdBy;
    private String updatedBy;
    private Boolean isDeleted;
    private LocalDateTime deletedAt;
    private String deletedBy;
    private Integer version;
    private String patientId;
    private String groupId;
    private String busNo;
    private String statusEnum;
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