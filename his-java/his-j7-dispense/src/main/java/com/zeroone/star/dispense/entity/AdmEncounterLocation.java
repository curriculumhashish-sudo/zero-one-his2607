package com.zeroone.star.dispense.entity;

import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Getter;
import lombok.Setter;

import java.io.Serializable;
import java.time.LocalDateTime;

@Getter
@Setter
@TableName("adm_encounter_location")
public class AdmEncounterLocation implements Serializable {
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
    private String encounterId;
    private String locationId;
    private String formEnum;
    private String statusEnum;
    private LocalDateTime startTime;
    private LocalDateTime endTime;
}
