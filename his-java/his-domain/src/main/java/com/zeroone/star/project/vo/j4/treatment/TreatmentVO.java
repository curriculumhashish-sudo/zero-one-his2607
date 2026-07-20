package com.zeroone.star.project.vo.j4.treatment;

import lombok.Data;

import java.time.LocalDateTime;

/**
 * <p>
 * 描述：诊疗管理视图对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@Data
public class TreatmentVO {

    private String id;

    private String tenantId;

    private LocalDateTime createdAt;

    private LocalDateTime updatedAt;

    private String createdBy;

    private String updatedBy;

    private Integer isDeleted;

    private LocalDateTime deletedAt;

    private String deletedBy;

    private Integer version;

    private String categoryCode;

    private String busNo;

    private String name;

    private String pyStr;

    private String wbStr;

    private String packageUnitCode;

    private String typeEnum;

    private String statusEnum;

    private String descriptionText;

    private String bodySiteCode;

    private String specimenCode;

    private String orgId;

    private String locationId;

    private Integer ruleId;

    private Integer pricingFlag;

    private Integer ybFlag;

    private String ybNo;

    private Integer ybMatchFlag;

    private Integer chrgitmLv;

    private String childrenJson;

    private Integer childrenFlag;

    private String activityVersion;

    private String merchandiseName;

    private String merchandisePyStr;

    private String merchandiseWbStr;

    private String defEncounterUnitCode;

    private String defInhospitalUnitCode;
}