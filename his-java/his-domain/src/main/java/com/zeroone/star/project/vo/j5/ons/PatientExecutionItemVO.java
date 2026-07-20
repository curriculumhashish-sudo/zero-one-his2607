package com.zeroone.star.project.vo.j5.ons;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

@Data
@Schema(description = "患者执行项目")
public class PatientExecutionItemVO {

    @Schema(description = "项目ID", example = "1")
    private String id;

    @Schema(description = "医嘱ID", example = "1")
    private String order_id;

    @Schema(description = "项目名称", example = "阿莫西林胶囊")
    private String item_name;

    @Schema(description = "项目编码", example = "MED001")
    private String item_code;

    @Schema(description = "项目类型", example = "MEDICATION")
    private String item_type;

    @Schema(description = "剂量", example = "0.5")
    private String dosage;

    @Schema(description = "剂量单位", example = "g")
    private String dosage_unit;

    @Schema(description = "执行频次", example = "每日三次")
    private String frequency;

    @Schema(description = "执行状态（0-未执行，1-已执行，2-已取消）", example = "0")
    private Integer status;

    @Schema(description = "执行状态名称", example = "未执行")
    private String status_name;

    @Schema(description = "执行者姓名", example = "李护士")
    private String executor_name;

    @Schema(description = "执行时间", example = "2024-01-15 10:30:00")
    private String execute_time;
}