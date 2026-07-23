package com.zeroone.star.project.vo.j4.treatment;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.io.Serializable;
import java.time.LocalDateTime;

@Data
@Schema(description = "诊疗项目详情显示对象")
public class TreatmentDetailVO implements Serializable {

    private static final long serialVersionUID = 1L;

    @Schema(description = "主键ID")
    private String id;

    @Schema(description = "租户ID")
    private String tenantId;

    @Schema(description = "创建时间")
    private LocalDateTime createdAt;

    @Schema(description = "更新时间")
    private LocalDateTime updatedAt;

    @Schema(description = "创建人")
    private String createdBy;

    @Schema(description = "更新人")
    private String updatedBy;

    @Schema(description = "逻辑删除标记，0未删除/1已删除")
    private Integer isDeleted;

    @Schema(description = "删除时间")
    private LocalDateTime deletedAt;

    @Schema(description = "删除人")
    private String deletedBy;

    @Schema(description = "乐观锁版本号")
    private Integer version;

    @Schema(description = "诊疗项目名称")
    private String name;

    @Schema(description = "服务类型编码")
    private String typeCode;

    @Schema(description = "分类编码")
    private String categoryCode;

    @Schema(description = "提供机构ID")
    private String offeredOrgId;

    @Schema(description = "专业科室编码")
    private String specialtyCode;

    @Schema(description = "执行地点ID")
    private String locationId;

    @Schema(description = "状态枚举：ACTIVE启用/INACTIVE停用")
    private String statusEnum;

    @Schema(description = "是否有效标记")
    private Boolean activeFlag;

    @Schema(description = "是否需要预约标记")
    private Boolean appointmentRequiredFlag;

    @Schema(description = "医保编码")
    private String ybNo;

    @Schema(description = "业务编号（编码）")
    private String busNo;

    @Schema(description = "备注说明")
    private String comment;

    @Schema(description = "扩展详情")
    private String extraDetails;

    @Schema(description = "联系方式")
    private String contact;
}
