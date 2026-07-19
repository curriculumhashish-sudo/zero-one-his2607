package com.zeroone.star.basedata.entity;

import com.baomidou.mybatisplus.annotation.TableName;
import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableField;
import com.baomidou.mybatisplus.annotation.*;
import lombok.Data;
import lombok.Getter;
import lombok.Setter;

import java.io.Serializable;
import java.time.LocalDateTime;

import java.time.LocalDateTime;

/**
 * <p>
 * 描述：诊疗管理实体
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@Data
@TableName("treatment")
public class Treatment {

    @TableId(value = "id", type = IdType.ASSIGN_UUID)
    private String id;

    @TableField("tenant_id")
    private String tenantId;

    @TableField("created_at")
    private LocalDateTime createdAt;

    @TableField("updated_at")
    private LocalDateTime updatedAt;

    @TableField("created_by")
    private String createdBy;

    @TableField("updated_by")
    private String updatedBy;

    @TableField("is_deleted")
    private Integer isDeleted;

    @TableField("deleted_at")
    private LocalDateTime deletedAt;

    @TableField("deleted_by")
    private String deletedBy;

    @TableField("version")
    private Integer version;

    @TableField("category_code")
    private String categoryCode;

    @TableField("bus_no")
    private String busNo;

    @TableField("name")
    private String name;

    @TableField("py_str")
    private String pyStr;

    @TableField("wb_str")
    private String wbStr;

    @TableField("package_unit_code")
    private String packageUnitCode;

    @TableField("type_enum")
    private String typeEnum;

    @TableField("status_enum")
    private String statusEnum;

    @TableField("description_text")
    private String descriptionText;

    @TableField("body_site_code")
    private String bodySiteCode;

    @TableField("specimen_code")
    private String specimenCode;

    @TableField("org_id")
    private String orgId;

    @TableField("location_id")
    private String locationId;

    @TableField("rule_id")
    private Integer ruleId;

    @TableField("pricing_flag")
    private Integer pricingFlag;

    @TableField("yb_flag")
    private Integer ybFlag;

    @TableField("yb_no")
    private String ybNo;

    @TableField("yb_match_flag")
    private Integer ybMatchFlag;

    @TableField("chrgitm_lv")
    private Integer chrgitmLv;

    @TableField("children_json")
    private String childrenJson;

    @TableField("children_flag")
    private Integer childrenFlag;

    @TableField("activity_version")
    private String activityVersion;

    @TableField("merchandise_name")
    private String merchandiseName;

    @TableField("merchandise_py_str")
    private String merchandisePyStr;

    @TableField("merchandise_wb_str")
    private String merchandiseWbStr;

    @TableField("def_encounter_unit_code")
    private String defEncounterUnitCode;

    @TableField("def_inhospital_unit_code")
    private String defInhospitalUnitCode;
}