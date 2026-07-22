package com.zeroone.star.setroster.entity;

import com.baomidou.mybatisplus.annotation.FieldFill;
import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableField;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableLogic;
import com.baomidou.mybatisplus.annotation.TableName;
import com.baomidou.mybatisplus.annotation.Version;
import lombok.Data;

import java.io.Serializable;
import java.time.LocalDateTime;

/**
 * 放号策略数据库实体，对应 bkg_release_policy 表。
 */
@Data
@TableName("bkg_release_policy")
public class BkgReleasePolicy implements Serializable {

    private static final long serialVersionUID = 1L;

    @TableId(value = "id", type = IdType.ASSIGN_ID)
    private String id;

    @TableField("tenant_id")
    private String tenantId;

    @TableField("organization_id")
    private String organizationId;

    @TableField("policy_code")
    private String policyCode;

    @TableField("policy_name")
    private String policyName;

    @TableField("advance_days")
    private Integer advanceDays;

    @TableField("slot_minutes")
    private Integer slotMinutes;

    @TableField("capacity_per_slot")
    private Integer capacityPerSlot;

    @TableField("channel_quota_json")
    private String channelQuotaJson;

    @TableField(value = "created_at", fill = FieldFill.INSERT)
    private LocalDateTime createdAt;

    @TableField(value = "updated_at", fill = FieldFill.INSERT_UPDATE)
    private LocalDateTime updatedAt;

    @TableField(value = "created_by", fill = FieldFill.INSERT)
    private String createdBy;

    @TableField(value = "updated_by", fill = FieldFill.INSERT_UPDATE)
    private String updatedBy;

    @TableLogic(value = "0", delval = "1")
    @TableField("is_deleted")
    private Integer isDeleted;

    @TableField("deleted_at")
    private LocalDateTime deletedAt;

    @TableField("deleted_by")
    private String deletedBy;

    @Version
    @TableField("version")
    private Integer version;
}
