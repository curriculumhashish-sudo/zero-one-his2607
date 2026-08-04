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

/**
 * <p>
 * 描述：放号策略数据库实体，对应 bkg_release_policy 表
 * </p>
 * <p>版权：01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author 余悸
 * @version 1.0.0
 */
@Data
@TableName("bkg_release_policy")
public class BkgReleasePolicy implements Serializable {

    private static final long serialVersionUID = 1L;

    @TableId(value = "id", type = IdType.ASSIGN_ID)// 主键ID
    private String id;

    @TableField("tenant_id")// 租户ID
    private String tenantId;

    @TableField("organization_id")// 组织ID
    private String organizationId;

    @TableField("policy_code")// 放号策略编码码
    private String policyCode;

    @TableField("policy_name")// 放号策略名称
    private String policyName;

    @TableField("advance_days")// 提前天数
    private Integer advanceDays;

    @TableField("slot_minutes")// 时段段分钟数
    private Integer slotMinutes;

    @TableField("capacity_per_slot")// 默认每时段段容量
    private Integer capacityPerSlot;

    @TableField("channel_quota_json")// 渠道配额JSON
    private String channelQuotaJson;

    @TableField(value = "created_at", fill = FieldFill.INSERT)// 创建时间
    private LocalDateTime createTime;

    @TableField(value = "updated_at", fill = FieldFill.INSERT_UPDATE)// 更新时间
    private LocalDateTime updateTime;

    @TableField(value = "created_by", fill = FieldFill.INSERT)// 创建人
    private String createdBy;

    @TableField(value = "updated_by", fill = FieldFill.INSERT_UPDATE)// 更新人
    private String updatedBy;

    @TableLogic(value = "0", delval = "1")// 逻辑删除标识
    @TableField("is_deleted")// 是否已删除
    private Integer isDeleted;

    @TableField("deleted_at")
    private LocalDateTime deletedAt;

    @TableField("deleted_by")// 删除人
    private String deletedBy;

    @Version
    @TableField("version")// 版本号
    private Integer version;
}
