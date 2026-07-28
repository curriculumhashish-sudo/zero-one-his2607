package com.zeroone.star.ons.entity;

import com.baomidou.mybatisplus.annotation.TableField;
import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Data;
import lombok.EqualsAndHashCode;

import java.math.BigDecimal;
import java.time.LocalDateTime;

@Data
@EqualsAndHashCode(callSuper = true)
@TableName("wkf_service_request")
public class WkfServiceRequest extends BaseEntity {

    @TableField("bus_no")
    private String busNo;

    @TableField("prescription_no")
    private String prescriptionNo;

    @TableField("based_on_table")
    private String basedOnTable;

    @TableField("based_on_id")
    private String basedOnId;

    @TableField("requisition_id")
    private String requisitionId;

    @TableField("status_enum")
    private String statusEnum;

    @TableField("intent_enum")
    private String intentEnum;

    @TableField("category_enum")
    private String categoryEnum;

    @TableField("group_id")
    private String groupId;

    @TableField("priority_enum")
    private String priorityEnum;

    @TableField("perform_flag")
    private Integer performFlag;

    @TableField("activity_id")
    private String activityId;

    @TableField("quantity")
    private BigDecimal quantity;

    @TableField("unit_code")
    private String unitCode;

    @TableField("patient_id")
    private String patientId;

    @TableField("encounter_id")
    private String encounterId;

    @TableField("authored_time")
    private LocalDateTime authoredTime;

    @TableField("requester_id")
    private String requesterId;

    @TableField("performer_type_code")
    private String performerTypeCode;

    @TableField("performer_id")
    private String performerId;

    @TableField("performer_check_id")
    private String performerCheckId;

    @TableField("check_time")
    private LocalDateTime checkTime;

    @TableField("location_id")
    private String locationId;

    @TableField("org_id")
    private String orgId;

    @TableField("reason_text")
    private String reasonText;

    @TableField("contract_id")
    private String contractId;

    @TableField("occurrence_start_time")
    private LocalDateTime occurrenceStartTime;

    @TableField("occurrence_end_time")
    private LocalDateTime occurrenceEndTime;

    @TableField("relevant_history_id")
    private String relevantHistoryId;

    @TableField("refund_service_id")
    private String refundServiceId;

    @TableField("content_json")
    private String contentJson;

    @TableField("yb_class_enum")
    private String ybClassEnum;

    @TableField("print_count")
    private Integer printCount;

    @TableField("condition_definition_id")
    private String conditionDefinitionId;

    @TableField("encounter_diagnosis_id")
    private String encounterDiagnosisId;

    @TableField("therapy_enum")
    private String therapyEnum;

    @TableField("rate_code")
    private String rateCode;

    @TableField("parent_id")
    private String parentId;

    @TableField("generate_source_enum")
    private String generateSourceEnum;

    @TableField("remark")
    private String remark;

    @TableField("entrust_content")
    private String entrustContent;

    @TableField("sort_number")
    private Integer sortNumber;

    @TableField("execution_count")
    private Integer executionCount;
}