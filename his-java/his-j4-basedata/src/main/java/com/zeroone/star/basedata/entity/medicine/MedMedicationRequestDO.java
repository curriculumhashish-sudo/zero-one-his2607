package com.zeroone.star.basedata.entity.medicine;

import com.baomidou.mybatisplus.annotation.FieldFill;
import com.baomidou.mybatisplus.annotation.TableField;
import com.baomidou.mybatisplus.annotation.TableLogic;
import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;
import com.baomidou.mybatisplus.annotation.Version;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

import java.io.Serializable;
import java.math.BigDecimal;
import java.time.LocalDateTime;

/**
 * <p>
 * 药品 medication request
 * </p>
 *
 * @author xxm
 * @since 2026-07-18
 */
@Getter
@Setter
@ToString
@TableName("med_medication_request")
public class MedMedicationRequestDO implements Serializable {

    private static final long serialVersionUID = 1L;

    /**
     * 主键
     */
    @TableId(value = "id", type = IdType.ASSIGN_ID)
    private String id;

    /**
     * 租户标识
     */
    @TableField("tenant_id")
    private String tenantId;

    /**
     * 创建时间
     */
    @TableField(value = "created_at",fill = FieldFill.INSERT)
    private LocalDateTime createdAt;

    /**
     * 最后更新时间
     */
    @TableField(value = "updated_at",fill = FieldFill.INSERT_UPDATE)
    private LocalDateTime updatedAt;

    /**
     * 创建人
     */
    @TableField(value = "created_by", fill = FieldFill.INSERT)
    private String createdBy;

    /**
     * 最后更新人
     */
    @TableField(value = "updated_by", fill = FieldFill.INSERT_UPDATE)
    private String updatedBy;

    /**
     * 软删标记
     */
    @TableLogic
    @TableField("is_deleted")
    private Boolean isDeleted;

    /**
     * 删除时间
     */
    @TableField("deleted_at")
    private LocalDateTime deletedAt;

    /**
     * 删除人
     */
    @TableField("deleted_by")
    private String deletedBy;

    /**
     * 乐观锁版本号
     */
    @Version
    @TableField("version")
    private Integer version;

    /**
     * 业务编号
     */
    @TableField("bus_no")
    private String busNo;

    /**
     * 处方编号
     */
    @TableField("prescription_no")
    private String prescriptionNo;

    /**
     * 分组标识
     */
    @TableField("group_id")
    private String groupId;

    /**
     * package标识
     */
    @TableField("package_id")
    private String packageId;

    /**
     * 状态枚举值
     */
    @TableField("status_enum")
    private String statusEnum;

    /**
     * 状态原因
     */
    @TableField("status_reason")
    private Integer statusReason;

    /**
     * 状态changed时间
     */
    @TableField("status_changed_time")
    private LocalDateTime statusChangedTime;

    /**
     * intent枚举值
     */
    @TableField("intent_enum")
    private String intentEnum;

    /**
     * category枚举值
     */
    @TableField("category_enum")
    private String categoryEnum;

    /**
     * priority枚举值
     */
    @TableField("priority_enum")
    private String priorityEnum;

    /**
     * perform标志位
     */
    @TableField("perform_flag")
    private Boolean performFlag;

    /**
     * 药品标识
     */
    @TableField("medication_id")
    private String medicationId;

    /**
     * 患者标识
     */
    @TableField("patient_id")
    private String patientId;

    /**
     * 就诊标识
     */
    @TableField("encounter_id")
    private String encounterId;

    /**
     * 药品申请聚合根
     */
    @TableField("requester_id")
    private String requesterId;

    /**
     * 位置/科室标识
     */
    @TableField("location_id")
    private String locationId;

    /**
     * 开立科室组织主键（{ adm_机构.id}）
     */
    @TableField("org_id")
    private String orgId;

    /**
     * 发放药房库位主键（{ adm_地点.id}）；与 { #orgId} 分列，禁止混存
     */
    @TableField("perform_location")
    private Long performLocation;

    /**
     * 执行机构
     */
    @TableField("perform_org")
    private Long performOrg;

    /**
     * performer枚举值
     */
    @TableField("performer_enum")
    private String performerEnum;

    /**
     * performer标识
     */
    @TableField("performer_id")
    private String performerId;

    /**
     * 设备def标识
     */
    @TableField("device_def_id")
    private String deviceDefId;

    /**
     * 记录人标识
     */
    @TableField("recorder_id")
    private String recorderId;

    /**
     * reqauthored时间
     */
    @TableField("req_authored_time")
    private LocalDateTime reqAuthoredTime;

    /**
     * 报表标志位
     */
    @TableField("report_flag")
    private Boolean reportFlag;

    /**
     * 辅助信息
     */
    @TableField("support_info")
    private String supportInfo;

    /**
     * 原因JSON
     */
    @TableField("reason_json")
    private String reasonJson;

    /**
     * therapy枚举值
     */
    @TableField("therapy_enum")
    private String therapyEnum;

    /**
     * effectivedose开始
     */
    @TableField("effective_dose_start")
    private LocalDateTime effectiveDoseStart;

    /**
     * 数量
     */
    @TableField("quantity")
    private BigDecimal quantity;

    /**
     * 执行次数
     */
    @TableField("execute_num")
    private Integer executeNum;

    /**
     * 单元编码
     */
    @TableField("unit_code")
    private String unitCode;

    /**
     * method编码
     */
    @TableField("method_code")
    private String methodCode;

    /**
     * rate编码
     */
    @TableField("rate_code")
    private String rateCode;

    /**
     * 剂量
     */
    @TableField("dose")
    private BigDecimal dose;

    /**
     * dose单元编码
     */
    @TableField("dose_unit_code")
    private String doseUnitCode;

    /**
     * 最大剂量
     */
    @TableField("max_dose")
    private BigDecimal maxDose;

    /**
     * 字段：first dose
     */
    @TableField("first_dose")
    private BigDecimal firstDose;

    /**
     * firstdose单元编码
     */
    @TableField("first_dose_unit_code")
    private String firstDoseUnitCode;

    /**
     * first耗时
     */
    @TableField("first_duration")
    private String firstDuration;

    /**
     * 发药interval
     */
    @TableField("dispense_interval")
    private String dispenseInterval;

    /**
     * 发药per数量
     */
    @TableField("dispense_per_quantity")
    private Integer dispensePerQuantity;

    /**
     * 发药per耗时
     */
    @TableField("dispense_per_duration")
    private Integer dispensePerDuration;

    /**
     * 批号
     */
    @TableField("lot_number")
    private String lotNumber;

    /**
     * 追踪编号
     */
    @TableField("trace_no")
    private String traceNo;

    /**
     * skintest标志位
     */
    @TableField("skin_test_flag")
    private Boolean skinTestFlag;

    /**
     * infusion标志位
     */
    @TableField("infusion_flag")
    private Boolean infusionFlag;

    /**
     * speed编码
     */
    @TableField("speed_code")
    private String speedCode;

    /**
     * 合同标识
     */
    @TableField("contract_id")
    private String contractId;

    /**
     * 诊断定义主键（列 { 病情_定义_id}，即 { cln_病情_定义.id}）
     */
    @TableField("condition_definition_id")
    private String conditionDefinitionId;

    /**
     * 就诊诊断标识
     */
    @TableField("encounter_diagnosis_id")
    private String encounterDiagnosisId;

    /**
     * performercheck标识
     */
    @TableField("performer_check_id")
    private String performerCheckId;

    /**
     * check时间
     */
    @TableField("check_time")
    private LocalDateTime checkTime;

    /**
     * effectivedose结束
     */
    @TableField("effective_dose_end")
    private LocalDateTime effectiveDoseEnd;

    /**
     * refundmedicine标识
     */
    @TableField("refund_medicine_id")
    private String refundMedicineId;

    /**
     * rx类型编码
     */
    @TableField("rx_type_code")
    private String rxTypeCode;

    /**
     * 医保class枚举值
     */
    @TableField("yb_class_enum")
    private String ybClassEnum;

    /**
     * 用法说明
     */
    @TableField("dosage_instruction")
    private String dosageInstruction;

    /**
     * 依据表名
     */
    @TableField("based_on_table")
    private String basedOnTable;

    /**
     * basedon标识
     */
    @TableField("based_on_id")
    private String basedOnId;

    /**
     * tcm标志位
     */
    @TableField("tcm_flag")
    private Boolean tcmFlag;

    /**
     * 内容JSON
     */
    @TableField("content_json")
    private String contentJson;

    /**
     * chineseherbsdose数量
     */
    @TableField("chinese_herbs_dose_quantity")
    private BigDecimal chineseHerbsDoseQuantity;

    /**
     * suffering标志位
     */
    @TableField("suffering_flag")
    private Boolean sufferingFlag;

    /**
     * 排序号
     */
    @TableField("sort_number")
    private Integer sortNumber;

    /**
     * generate来源枚举值
     */
    @TableField("generate_source_enum")
    private String generateSourceEnum;

    /**
     * 备注
     */
    @TableField("remark")
    private String remark;
}
