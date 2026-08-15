package com.zeroone.star.sysmgr.entity.dept;

import com.baomidou.mybatisplus.annotation.*;
import com.zeroone.star.project.entity.BaseDO;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

import java.io.Serializable;
import java.time.LocalDateTime;

@Getter
@Setter
@ToString
@TableName("whale_organization_units")
public class WhaleOrganizationUnitDO extends BaseDO implements Serializable {

    /** 编码 */
    private String code;

    /** 显示名称（科室名称） */
    private String displayName;

    /** 父节点标识 */
    private String parentId;

    /** 排序号 */
    private Integer sortOrder;

    /** 是否启用 1启用 0禁用 */
    private Integer isEnabled;

    /** 备注 */
    private String remark;

    /** 租户标识 */
    private String tenantId;

    /** 逻辑删除标记 0未删除 1已删除 */
    @TableLogic
    private Integer isDeleted;

    /** 删除时间 */
    private LocalDateTime deletedAt;

    /** 删除人 */
    private String deletedBy;
}
