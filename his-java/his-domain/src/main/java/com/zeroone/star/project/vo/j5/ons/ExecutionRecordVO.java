package com.zeroone.star.project.vo.j5.ons;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

@Data
@Schema(description = "执行记录")
public class ExecutionRecordVO {

    @Schema(description = "记录ID", example = "1")
    private String id;

    @Schema(description = "患者ID", example = "1")
    private String patient_id;

    @Schema(description = "患者姓名", example = "张三")
    private String patient_name;

    @Schema(description = "患者性别", example = "男")
    private String patient_gender;

    @Schema(description = "患者年龄", example = "30")
    private Integer patient_age;

    @Schema(description = "医嘱ID", example = "1")
    private String order_id;

    @Schema(description = "医嘱内容", example = "阿莫西林胶囊 0.5g 口服 每日三次")
    private String order_content;

    @Schema(description = "执行状态（0-未执行，1-已执行，2-已取消）", example = "1")
    private Integer status;

    @Schema(description = "执行状态名称", example = "已执行")
    private String status_name;

    @Schema(description = "执行者ID", example = "1")
    private String executor_id;

    @Schema(description = "执行者姓名", example = "李护士")
    private String executor_name;

    @Schema(description = "执行时间", example = "2024-01-15 10:30:00")
    private String execute_time;

    @Schema(description = "备注", example = "执行完成")
    private String remark;

    @Schema(description = "创建时间", example = "2024-01-15 09:00:00")
    private String created_at;
}