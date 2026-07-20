package com.zeroone.star.project.query.j7.outpharm;

import com.zeroone.star.project.query.PageQuery;
import lombok.Data;
import lombok.EqualsAndHashCode;

/**
 * 患者药品项目 数据库查询条件
 */
@Data
@EqualsAndHashCode(callSuper = true)
public class PatientDrugItemQuery extends PageQuery {
    private String tenantId;
    private String patientId;
    private String encounterId;
    private String dispenseEnum;
    private String statusEnum;
    private String busNo;
}
