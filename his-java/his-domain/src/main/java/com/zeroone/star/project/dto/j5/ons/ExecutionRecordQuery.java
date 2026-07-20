package com.zeroone.star.project.dto.j5.ons;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import lombok.EqualsAndHashCode;

@Data
@EqualsAndHashCode(callSuper = true)
@Schema(description = "执行记录查询条件")
public class ExecutionRecordQuery extends PageQuery {

    @Schema(description = "患者ID", example = "1")
    private String patient_id;

    @Schema(description = "患者姓名", example = "张三")
    private String patient_name;

    @Schema(description = "执行状态（0-未执行，1-已执行，2-已取消）", example = "0")
    private Integer status;

    @Schema(description = "执行类型", example = "MEDICATION")
    private String execute_type;

    @Schema(description = "开始日期", example = "2024-01-01")
    private String start_date;

    @Schema(description = "结束日期", example = "2024-01-31")
    private String end_date;
}