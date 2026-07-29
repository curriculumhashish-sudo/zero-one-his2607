package com.zeroone.star.project.vo.j7.refundmedicine;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.time.LocalDateTime;

/**
 * <p>
 * 描述：住院退药-患者队列显示对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j7
 * @version 1.0.0
 */
@Data
@Schema(description = "门诊退药-患者队列显示对象")
public class OutpatientRefundPatientVO {

    /**
     * 患者ID
     */
    @Schema(description = "患者ID", example = "326940849401761792", requiredMode = Schema.RequiredMode.REQUIRED)
    private String patientId;

    /**
     * 就诊ID
     */
    @Schema(description = "就诊ID", example = "329874660147597312", requiredMode = Schema.RequiredMode.REQUIRED)
    private String encounterId;

    /**
     * 患者姓名（对应 adm_patient.name）
     */
    @Schema(description = "患者姓名", example = "李四", requiredMode = Schema.RequiredMode.REQUIRED)
    private String name;

    /**
     * 性别（对应 adm_patient.gender_enum）
     */
    @Schema(description = "性别", example = "1")
    private String genderEnum;

    /**
     * 住院号（对应 adm_encounter.bus_no）
     */
    @Schema(description = "住院号", example = "ZY20260719001", requiredMode = Schema.RequiredMode.REQUIRED)
    private String busNo;

    /**
     * 病区/科室ID
     */
    @Schema(description = "病区/科室ID", example = "322963602326687744")
    private String orgId;

    /**
     * 病区名称
     */
    @Schema(description = "病区名称", example = "内科病区")
    private String wardName;

    /**
     * 药房位置ID
     */
    @Schema(description = "药房位置ID", example = "322663218269917184")
    private String locationId;

    /**
     * 药房名称
     */
    @Schema(description = "药房名称", example = "住院药房")
    private String pharmacyName;

    /**
     * 待退药条数
     */
    @Schema(description = "待退药条数", example = "3")
    private Integer refundCount;

    /**
     * 最近申请时间
     */
    @Schema(description = "最近申请时间", example = "2026-07-19 10:30:00")
    private LocalDateTime latestApplyTime;
}
