package com.zeroone.star.project.dto.j4.treatment;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.Size;
import lombok.Data;

/**
 * 新增诊疗项目DTO
 */
@Data
@Schema(description = "新增诊疗项目DTO")
public class TreatmentAddDTO {

    @NotBlank(message = "诊疗项目名称不能为空")
    @Size(max = 255, message = "诊疗项目名称不能超过255个字符")
    @Schema(description = "诊疗项目名称", example = "血常规检查", requiredMode = Schema.RequiredMode.REQUIRED)
    private String name;

    @Schema(description = "服务类型编码", example = "EXAM")
    private String typeCode;

    @NotBlank(message = "分类编码不能为空")
    @Schema(description = "分类编码", example = "检查", requiredMode = Schema.RequiredMode.REQUIRED)
    private String categoryCode;

    @Schema(description = "提供机构ID", example = "1001")
    private String offeredOrgId;

    @Schema(description = "专业科室编码", example = "CARDIOLOGY")
    private String specialtyCode;

    @Schema(description = "执行地点ID", example = "2001")
    private String locationId;

    @Schema(description = "状态枚举：ACTIVE启用/INACTIVE停用", example = "ACTIVE")
    private String statusEnum;

    @Schema(description = "是否有效标记", example = "true")
    private Boolean activeFlag;

    @Schema(description = "是否需要预约标记", example = "false")
    private Boolean appointmentRequiredFlag;

    @Schema(description = "医保编码", example = "C010101001")
    private String ybNo;

    @NotBlank(message = "业务编号不能为空")
    @Schema(description = "业务编号（编码）", example = "CG001", requiredMode = Schema.RequiredMode.REQUIRED)
    private String busNo;

    @Size(max = 500, message = "备注说明不能超过500个字符")
    @Schema(description = "备注说明", example = "常规检查项目")
    private String comment;

    @Size(max = 2000, message = "扩展详情不能超过2000个字符")
    @Schema(description = "扩展详情", example = "{\"detail\":\"血常规全套\"}")
    private String extraDetails;

    @Size(max = 500, message = "联系方式不能超过500个字符")
    @Schema(description = "联系方式", example = "{\"phone\":\"12345678\"}")
    private String contact;
}
