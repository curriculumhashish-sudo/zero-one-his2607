package com.zeroone.star.project.vo.j7.dispense;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * <p>
 * 描述：住院退药-患者队列显示对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author jacobzp
 * @version 1.0.0
 */
@Data
@Schema(description = "住院退药-患者队列显示对象")
public class InpatientRefundPatientVO {

    /**
     * 就诊ID（对应 adm_encounter.id / med_medication_dispense.encounter_id；
     * 前端选中后请使用该值作为 get-refund-detail 接口的 encounterId 参数查询退药明细）
     */
    @Schema(description = "就诊ID（作为 get-refund-detail 接口的 encounterId 参数使用）", example = "330316881840836608", requiredMode = Schema.RequiredMode.REQUIRED)
    private String encounterId;

    /**
     * 患者姓名（对应 adm_patient.name）
     */
    @Schema(description = "患者姓名", example = "张三", requiredMode = Schema.RequiredMode.REQUIRED)
    private String patientName;

    /**
     * 性别编码（对应 adm_patient.gender_enum）
     */
    @Schema(description = "性别编码", example = "1")
    private String genderEnum;

    /**
     * 性别文案（由 adm_patient.gender_enum 转义，无直接表字段）
     */
    @Schema(description = "性别文案", example = "男")
    private String genderEnumText;

    /**
     * 年龄文案（由 adm_patient.birth_date 计算得出，无直接表字段）
     */
    @Schema(description = "年龄文案", example = "45岁")
    private String ageLabel;

    /**
     * 病区名称（对应 adm_location.name，form_enum=WARD；经 adm_encounter_location.location_id 关联就诊病区）
     */
    @Schema(description = "病区名称", example = "内科病区")
    private String wardName;

    /**
     * 床位名称（对应 adm_location.name，form_enum=BED；经 adm_encounter_location.location_id 关联就诊床位）
     */
    @Schema(description = "床位名称", example = "08床")
    private String bedName;

    /**
     * 住院号（对应 adm_encounter.bus_no）
     */
    @Schema(description = "住院号", example = "ZY20260719001", requiredMode = Schema.RequiredMode.REQUIRED)
    private String hospitalizationNo;

    /**
     * 退药状态码（对应 med_medication_dispense.status_enum；前置条件 dispense_enum=REFUND；
     * PENDING_REFUND=待退药，REFUNDED=已退药，DECLINED=退药拒绝）
     */
    @Schema(description = "退药状态码", example = "PENDING_REFUND")
    private String refundEnum;

    /**
     * 退药状态文案（由 med_medication_dispense.status_enum 转义，无直接表字段）
     */
    @Schema(description = "退药状态文案", example = "待退药")
    private String refundEnumText;
}
