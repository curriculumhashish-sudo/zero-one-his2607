package com.zeroone.star.setroster.entity;

import com.baomidou.mybatisplus.annotation.*;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

import java.io.Serializable;
import java.time.LocalDateTime;

/**
 * <p>
 * T1 放号时段模板（07 §5.2）
 * </p>
 *
 * @author Gin
 * @since 2026-07-24
 */
@Getter
@Setter
@ToString
@TableName("bkg_slot_provision_template")
public class BkgSlotProvisionTemplate implements Serializable {

    private static final long serialVersionUID = 1L;

    @TableId(value = "id", type = IdType.ASSIGN_ID)
    private String id;

    private String tenantId;

    private String organizationId;

    private String templateCode;

    private String templateName;

    private String slotTemplatesJson;

    private Boolean enabled;

    @TableField(fill = FieldFill.INSERT)
    private LocalDateTime createdAt;

    @TableField(fill = FieldFill.INSERT_UPDATE)
    private LocalDateTime updatedAt;

    @TableField(fill = FieldFill.INSERT)
    private String createdBy;

    @TableField(fill = FieldFill.INSERT_UPDATE)
    private String updatedBy;

    /**
     * 软删除标记（@TableLogic 使查询自动过滤已删除记录，removeById 自动转为 update）
     */
    @TableLogic
    private Boolean isDeleted;

    private LocalDateTime deletedAt;

    private String deletedBy;

    /**
     * 乐观锁版本号（配合全局 OptimisticLockerInnerInterceptor 实现并发控制）
     */
    @Version
    private Integer version;
}
