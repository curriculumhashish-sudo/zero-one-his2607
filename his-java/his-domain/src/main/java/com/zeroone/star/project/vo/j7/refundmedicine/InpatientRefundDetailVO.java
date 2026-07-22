package com.zeroone.star.project.vo.j7.refundmedicine;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.math.BigDecimal;
import java.time.LocalDateTime;

/**
 * <p>
 * 描述：住院退药-患者退药明细显示对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j7
 * @version 1.0.0
 */
@Data
@Schema(description = "住院退药-患者退药明细显示对象")
public class InpatientRefundDetailVO {

    /**
     * 退药申请ID（对应 med_medication_request.id）
     */
    @Schema(description = "退药申请ID", example = "329906419476205568", requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;

    /**
     * 关联原发药申请ID（对应 med_medication_request.refund_medicine_id）
     */
    @Schema(description = "关联原发药申请ID", example = "329891428408758272")
    private String refundMedicineId;

    /**
     * 患者ID
     */
    @Schema(description = "患者ID", example = "326940849401761792")
    private String patientId;

    /**
     * 就诊ID
     */
    @Schema(description = "就诊ID", example = "329874660147597312")
    private String encounterId;

    /**
     * 药品ID（对应 med_medication_request.medication_id）
     */
    @Schema(description = "药品ID", example = "322997883812974592", requiredMode = Schema.RequiredMode.REQUIRED)
    private String medicationId;

    /**
     * 药品名称（对应 med_medication_definition.name）
     */
    @Schema(description = "药品名称", example = "阿莫西林胶囊", requiredMode = Schema.RequiredMode.REQUIRED)
    private String medicationName;

    /**
     * 药品规格/商品名（对应 med_medication_definition.merchandise_name）
     */
    @Schema(description = "药品规格/商品名", example = "0.25g*24粒")
    private String merchandiseName;

    /**
     * 药品分类编码
     */
    @Schema(description = "药品分类编码", example = "western")
    private String categoryCode;

    /**
     * 医嘱类型（对应 med_medication_request.therapy_enum）
     */
    @Schema(description = "医嘱类型", example = "TEMPORARY")
    private String therapyEnum;

    /**
     * 退药数量（对应 med_medication_request.quantity）
     */
    @Schema(description = "退药数量", example = "1.00", requiredMode = Schema.RequiredMode.REQUIRED)
    private BigDecimal quantity;

    /**
     * 单位编码（对应 med_medication_request.unit_code）
     */
    @Schema(description = "单位编码", example = "8")
    private String unitCode;

    /**
     * 批号（对应 med_medication_request.lot_number / med_medication_dispense.lot_number）
     */
    @Schema(description = "批号", example = "20260501")
    private String lotNumber;

    /**
     * 用法说明（对应 med_medication_request.dosage_instruction）
     */
    @Schema(description = "用法说明", example = "口服，一日三次")
    private String dosageInstruction;

    /**
     * 给药途径编码（对应 med_medication_request.method_code）
     */
    @Schema(description = "给药途径编码", example = "404")
    private String methodCode;

    /**
     * 状态（对应 med_medication_request.status_enum）
     */
    @Schema(description = "状态", example = "PENDING_REFUND")
    private String statusEnum;

    /**
     * 申请时间（对应 med_medication_request.req_authored_time）
     */
    @Schema(description = "申请时间", example = "2026-07-19 10:30:00")
    private LocalDateTime reqAuthoredTime;

    /**
     * 业务编号（对应 med_medication_request.bus_no）
     */
    @Schema(description = "业务编号", example = "MR2026-07-190001")
    private String busNo;
}
