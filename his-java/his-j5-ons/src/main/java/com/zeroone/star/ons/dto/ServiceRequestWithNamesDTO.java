package com.zeroone.star.ons.dto;

import com.zeroone.star.ons.entity.WkfServiceRequest;
import lombok.Data;
import lombok.EqualsAndHashCode;

/**
 * 执行记录 + 关联名称 JOIN 查询结果。
 * 一次性带出：项目名称、申请医生姓名、执行护士姓名、科室名称。
 */
@Data
@EqualsAndHashCode(callSuper = true)
public class ServiceRequestWithNamesDTO extends WkfServiceRequest {

    /** 项目名称（adm_healthcare_service.name 或 med_medication_definition.name） */
    private String itemName;

    /** 申请人姓名（adm_practitioner.name via requester_id） */
    private String requesterName;

    /** 执行人姓名（adm_practitioner.name via performer_id） */
    private String performerName;

    /** 科室名称（adm_location.name via location_id） */
    private String locationName;
}
