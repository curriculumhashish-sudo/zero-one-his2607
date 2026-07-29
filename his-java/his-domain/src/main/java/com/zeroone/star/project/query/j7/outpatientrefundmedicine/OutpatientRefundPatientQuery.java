package com.zeroone.star.project.query.j7.refundmedicine;

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
 * @author j7
 * @version 1.0.0
 */
@Getter
@Setter
@ToString
@Schema(description = "门诊退药-患者队列查询对象")
public class OutpatientRefundPatientQuery extends PageQuery {

    /**
     * 药房位置ID（对应 med_medication_request.location_id / perform_location）
     */
    @Schema(description = "药房位置ID", example = "322663218269917184")
    private String locationId;

    /**
     * 病区/科室ID（对应 med_medication_request.org_id）
     */
    @Schema(description = "病区/科室ID", example = "322963602326687744")
    private String orgId;

    /**
     * 申请开始日期
     */
    @Schema(description = "申请开始日期", example = "2026-07-01")
    private String applyStartDate;

    /**
     * 申请结束日期
     */
    @Schema(description = "申请结束日期", example = "2026-07-19")
    private String applyEndDate;

    /**
     * 患者关键字（姓名/住院号）
     */
    @Schema(description = "患者关键字（姓名/住院号）", example = "张三")
    private String keyword;
}
