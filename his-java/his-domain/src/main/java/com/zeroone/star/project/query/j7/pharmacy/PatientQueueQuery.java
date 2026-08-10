package com.zeroone.star.project.query.j7.pharmacy;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

/**
 * <p>
 * 描述：患者队列查询对象（支持分页+查询条件）
 * </p>
 * <p>
 * 功能说明：用于分页查询退药患者队列时的参数封装。
 * 继承 PageQuery 基础分页类，增加了按患者姓名（模糊查询）、
 * 病历号、科室名称、单据状态等条件过滤。
 * 在住院退药确认和门诊拒绝退药中均使用该查询对象。
 * 数据查询基于 return_header 表。
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 01star
 * @version 1.0.0
 */
@Getter
@Setter
@ToString
public class PatientQueueQuery extends PageQuery {

    @Schema(description = "患者姓名（模糊查询）", example = "周女士")
    private String patientName;

    @Schema(description = "病历号", example = "ZY2026-06-300001")
    private String medicalNo;

    @Schema(description = "科室名称", example = "住院内科")
    private String deptName;

    @Schema(description = "单据状态（0-待处理 1-已处理）", example = "0")
    private Integer status;
}
