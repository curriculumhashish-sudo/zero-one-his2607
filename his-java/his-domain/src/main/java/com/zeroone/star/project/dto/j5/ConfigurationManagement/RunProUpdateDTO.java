package com.zeroone.star.project.dto.j5.ConfigurationManagement;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.Size;
import lombok.Data;

/**
 * 执行项目修改DTO（映射adm_healthcare_service表）
 */
@Data
@Schema(description = "执行项目修改DTO")
public class RunProUpdateDTO {

    @NotBlank(message = "项目ID不能为空")
    @Schema(description = "项目ID", requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;

    @Size(max = 255, message = "项目名称不能超过255个字符")
    @Schema(description = "项目名称")
    private String name;

    @Size(max = 255, message = "类型编码不能超过255个字符")
    @Schema(description = "类型编码")
    private String typeCode;

    @Schema(description = "分类编码")
    private String categoryCode;

    @Schema(description = "执行机构ID")
    private String offeredOrgId;

    @Schema(description = "专科编码")
    private String specialtyCode;

    @Schema(description = "执行地点ID")
    private String locationId;

    @Schema(description = "状态枚举")
    private String statusEnum;

    @Schema(description = "启用标志")
    private Integer activeFlag;

    @Schema(description = "是否需要预约")
    private Integer appointmentRequiredFlag;

    @Schema(description = "医保编号")
    private String ybNo;

    @Schema(description = "业务编号")
    private String busNo;

    @Schema(description = "备注")
    private String comment;

    @Schema(description = "扩展详情")
    private String extraDetails;

    @Schema(description = "联系方式")
    private String contact;
}
