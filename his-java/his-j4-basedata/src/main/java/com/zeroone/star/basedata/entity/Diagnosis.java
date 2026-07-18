package com.zeroone.star.basedata.entity;

import com.baomidou.mybatisplus.annotation.*;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.time.LocalDateTime;

@Data
@AllArgsConstructor
@NoArgsConstructor
@TableName("mrms_homepage_diagnosis")
public class Diagnosis {

    @TableId(value = "id", type = IdType.ASSIGN_UUID)
    private String id;

    @TableField("case_record_id")
    private String caseRecordId;

    @TableField("tenant_id")
    private String tenantId;

    @TableField("diag_type_code")
    private String diagTypeCode;

    @TableField("sort_no")
    private Integer sortNo;

    @TableField("diag_code")
    private String diagCode;

    @TableField("diag_name")
    private String diagName;

    @TableField("adm_cond_code")
    private String admCondCode;

    @TableField("maindiag_flag")
    private String maindiagFlag;

    @TableField("vali_flag")
    private String valiFlag;

    @TableField(value = "created_at", fill = FieldFill.INSERT)
    private LocalDateTime createdAt;

    @TableField(value = "updated_at", fill = FieldFill.INSERT_UPDATE)
    private LocalDateTime updatedAt;

    @TableField("created_by")
    private String createdBy;

    @TableField("updated_by")
    private String updatedBy;

    @TableLogic
    @TableField("is_deleted")
    private Integer isDeleted;

    @TableField("deleted_at")
    private LocalDateTime deletedAt;

    @TableField("deleted_by")
    private String deletedBy;
}