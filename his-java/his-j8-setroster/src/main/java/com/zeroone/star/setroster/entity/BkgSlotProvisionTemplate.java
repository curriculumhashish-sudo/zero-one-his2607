package com.zeroone.star.setroster.entity;

import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;
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

    private LocalDateTime createdAt;

    private LocalDateTime updatedAt;

    private String createdBy;

    private String updatedBy;

    private Boolean isDeleted;

    private LocalDateTime deletedAt;

    private String deletedBy;

    private Integer version;
}
