package com.zeroone.star.project.query.j7.dispense;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

/**
 * <p>
 * 描述：住院退药-患者队列查询对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author jacobzp
 * @version 1.0.0
 */
@Getter
@Setter
@ToString
@Schema(description = "住院退药-患者队列查询对象")
public class InpatientRefundPatientQuery extends PageQuery {

    /**
     * 病区ID（对应 adm_location.id，form_enum=WARD；经 adm_encounter_location.location_id 关联就诊病区）
     */
    @Schema(description = "病区ID", example = "322963602326687744")
    private String wardId;

    /**
     * 队列页签：PENDING-待退药 / COMPLETED-已退药
     * （前置条件 med_medication_dispense.dispense_enum=REFUND；
     *  对应 med_medication_dispense.status_enum：PENDING=待退药，REFUNDED=已退药，DECLINED=退药拒绝；
     *  页签 COMPLETED 映射 status_enum=REFUNDED）
     */
    @Schema(description = "队列页签（PENDING/COMPLETED）", example = "PENDING")
    private String queueTab;

    /**
     * 患者关键字（姓名/住院号/床号）（对应 adm_patient.name / adm_encounter.bus_no / adm_location.name(form_enum=BED)）
     */
    @Schema(description = "患者关键字（姓名/住院号/床号）", example = "张三")
    private String keyword;
}
