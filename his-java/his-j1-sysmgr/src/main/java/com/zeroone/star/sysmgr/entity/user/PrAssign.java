package com.zeroone.star.sysmgr.entity.user;

import com.baomidou.mybatisplus.annotation.TableName;

import java.time.LocalDateTime;

import com.zeroone.star.project.entity.BaseDO;
import lombok.*;

/**
 * <p>
 * 管理域 PR分配
 * </p>
 *
 * @author sykesolune
 * @since 2026-07-27
 */
@Getter
@Setter
@ToString
@TableName("adm_pr_assign")
public class PrAssign extends BaseDO {

    /**
     * 租户标识
     */
    private String tenantId;

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
    private Integer version;

    /**
     * 名称
     */
    private String name;

    /**
     * 是否启用
     */
    private Boolean activeFlag;

    /**
     * 医护人员标识
     */
    private String practitionerId;

    /**
     * 机构标识
     */
    private String orgId;

    /**
     * 位置/科室标识
     */
    private String locationId;

    /**
     * specialty编码
     */
    private String specialtyCode;

    /**
     * 类型编码
     */
    private Integer typeCode;

    /**
     * 岗位分配聚合根（{ adm_pr_assign}）
     */
    private String roleId;
}
