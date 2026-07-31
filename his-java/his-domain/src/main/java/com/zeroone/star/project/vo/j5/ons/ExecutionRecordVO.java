package com.zeroone.star.project.vo.j5.ons;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.math.BigDecimal;

@Data
@Schema(description = "服务请求")
public class ExecutionRecordVO {

    @Schema(description = "主键ID", example = "1")
    private String id;

    @Schema(description = "业务编号", example = "SR20240115001")
    private String bus_no;

    @Schema(description = "处方编号", example = "P20240115001")
    private String prescription_no;

    @Schema(description = "依据表名", example = "wkf_service_request")
    private String based_on_table;

    @Schema(description = "依据标识", example = "1")
    private String based_on_id;

    @Schema(description = "状态枚举值", example = "ACTIVE")
    private String status_enum;

    @Schema(description = "状态名称", example = "活动")
    private String status_name;

    @Schema(description = "意图枚举值", example = "ORDER")
    private String intent_enum;

    @Schema(description = "分类枚举值", example = "MEDICATION")
    private String category_enum;

    @Schema(description = "分类名称", example = "药品")
    private String category_name;

    @Schema(description = "优先级枚举值", example = "ROUTINE")
    private String priority_enum;

    @Schema(description = "优先级名称", example = "普通")
    private String priority_name;

    @Schema(description = "执行标志位", example = "1")
    private Integer perform_flag;

    @Schema(description = "活动标识", example = "1")
    private String activity_id;

    @Schema(description = "项目名称", example = "血压测量")
    private String item_name;

    @Schema(description = "数量", example = "1.00")
    private BigDecimal quantity;

    @Schema(description = "单元编码", example = "BOX")
    private String unit_code;

    @Schema(description = "单位名称", example = "盒")
    private String unit_name;

    @Schema(description = "患者标识", example = "1")
    private String patient_id;

    @Schema(description = "患者姓名", example = "张三")
    private String patient_name;

    @Schema(description = "患者性别", example = "MALE")
    private String patient_gender;

    @Schema(description = "就诊标识", example = "1")
    private String encounter_id;

    @Schema(description = "开立时间", example = "2024-01-15 09:00:00")
    private String authored_time;

    @Schema(description = "申请人标识", example = "1")
    private String requester_id;

    @Schema(description = "申请人姓名", example = "张医生")
    private String requester_name;

    @Schema(description = "执行者类型编码", example = "NURSE")
    private String performer_type_code;

    @Schema(description = "执行者标识", example = "1")
    private String performer_id;

    @Schema(description = "执行者姓名", example = "护士小王")
    private String performer_name;

    @Schema(description = "执行者核对标识", example = "1")
    private String performer_check_id;

    @Schema(description = "核对时间", example = "2024-01-15 10:00:00")
    private String check_time;

    @Schema(description = "位置/科室标识", example = "1")
    private String location_id;

    @Schema(description = "位置/科室名称", example = "内科")
    private String location_name;

    @Schema(description = "机构标识", example = "1")
    private String org_id;

    @Schema(description = "原因文本", example = "感冒")
    private String reason_text;

    @Schema(description = "执行开始时间", example = "2024-01-15 10:30:00")
    private String occurrence_start_time;

    @Schema(description = "执行结束时间", example = "2024-01-15 11:00:00")
    private String occurrence_end_time;

    @Schema(description = "执行日期", example = "2024-01-15")
    private String occurrence_date;

    @Schema(description = "内容JSON", example = "{\"dose\":\"500mg\"}")
    private String content_json;

    @Schema(description = "医保分类枚举值", example = "MEDICAL_INSURANCE")
    private String yb_class_enum;

    @Schema(description = "打印次数", example = "1")
    private Integer print_count;

    @Schema(description = "备注", example = "注意事项")
    private String remark;

    @Schema(description = "嘱托内容", example = "饭后服用")
    private String entrust_content;

    @Schema(description = "执行次数", example = "1")
    private Integer execution_count;

    @Schema(description = "执行状态（0-未执行，1-已执行，2-已取消）", example = "1")
    private Integer execution_status;

    @Schema(description = "执行状态名称", example = "已执行")
    private String execution_status_name;

    @Schema(description = "创建时间", example = "2024-01-15 09:00:00")
    private String created_at;
}