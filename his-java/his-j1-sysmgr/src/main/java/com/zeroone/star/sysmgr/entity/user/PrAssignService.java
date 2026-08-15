package com.zeroone.star.sysmgr.entity.user;

import com.baomidou.mybatisplus.annotation.TableField;
import com.baomidou.mybatisplus.annotation.TableName;

import java.time.LocalDateTime;

import com.zeroone.star.project.entity.BaseDO;
import lombok.*;

/**
 * <p>
 * 管理域 PR分配服务
 * </p>
 *
 * @author sykesolune
 * @since 2026-07-27
 */
@Getter
@Setter
@ToString
@TableName("adm_pr_assign_service")
public class PrAssignService extends BaseDO {
    /**
     * 租户标识
     */
    @TableField("tenant_id")
    private String tenantId;

    /**
     * 删除时间
     */
    @TableField("deleted_at")
    private LocalDateTime deletedAt;

    /**
     * 删除人
     */
    @TableField("deleted_by")
    private String deletedBy;

    /**
     * 乐观锁版本号
     */
    @TableField("version")
    private Integer version;

    /**
     * 岗位服务项子实体，对应表 { adm_pr_assign_service}
     */
    @TableField("assignment_id")
    private String assignmentId;

    /**
     * 医疗服务标识，关联 { adm_healthcare_service.id}
     */
    @TableField("healthcare_service_id")
    private String healthcareServiceId;

}
