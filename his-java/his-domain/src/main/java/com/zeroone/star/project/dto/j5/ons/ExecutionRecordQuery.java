package com.zeroone.star.project.dto.j5.ons;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import lombok.EqualsAndHashCode;

@Data
@EqualsAndHashCode(callSuper = true)
@Schema(description = "服务请求查询条件")
public class ExecutionRecordQuery extends PageQuery {

    @Schema(description = "患者ID", example = "1")
    private String patient_id;

    @Schema(description = "就诊标识", example = "1")
    private String encounter_id;

    @Schema(description = "状态枚举值", example = "ACTIVE")
    private String status_enum;

    @Schema(description = "分类枚举值", example = "MEDICATION")
    private String category_enum;

    @Schema(description = "优先级枚举值", example = "ROUTINE")
    private String priority_enum;

    @Schema(description = "处方编号", example = "P20240115001")
    private String prescription_no;

    @Schema(description = "开始日期", example = "2024-01-01")
    private String start_date;

    @Schema(description = "结束日期", example = "2024-01-31")
    private String end_date;
}