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
    private String busNo;

    @Schema(description = "处方编号", example = "P20240115001")
    private String prescriptionNo;

    @Schema(description = "依据表名", example = "wkf_service_request")
    private String basedOnTable;

    @Schema(description = "依据标识", example = "1")
    private String basedOnId;

    @Schema(description = "状态枚举值", example = "ACTIVE")
    private String statusEnum;

    @Schema(description = "状态名称", example = "活动")
    private String statusName;

    @Schema(description = "意图枚举值", example = "ORDER")
    private String intentEnum;

    @Schema(description = "分类枚举值", example = "MEDICATION")
    private String categoryEnum;

    @Schema(description = "分类名称", example = "药品")
    private String categoryName;

    @Schema(description = "优先级枚举值", example = "ROUTINE")
    private String priorityEnum;

    @Schema(description = "优先级名称", example = "普通")
    private String priorityName;

    @Schema(description = "执行标志位", example = "1")
    private Integer performFlag;

    @Schema(description = "活动标识", example = "1")
    private String activityId;

    @Schema(description = "项目名称", example = "血压测量")
    private String itemName;

    @Schema(description = "数量", example = "1.00")
    private BigDecimal quantity;

    @Schema(description = "单元编码", example = "BOX")
    private String unitCode;

    @Schema(description = "单位名称", example = "盒")
    private String unitName;

    @Schema(description = "患者标识", example = "1")
    private String patientId;

    @Schema(description = "患者姓名", example = "张三")
    private String patientName;

    @Schema(description = "患者性别", example = "MALE")
    private String patientGender;

    @Schema(description = "就诊标识", example = "1")
    private String encounterId;

    @Schema(description = "开立时间", example = "2024-01-15 09:00:00")
    private String authoredTime;

    @Schema(description = "申请人标识", example = "1")
    private String requesterId;

    @Schema(description = "申请人姓名", example = "张医生")
    private String requesterName;

    @Schema(description = "执行者类型编码", example = "NURSE")
    private String performerTypeCode;

    @Schema(description = "执行者标识", example = "1")
    private String performerId;

    @Schema(description = "执行者姓名", example = "护士小王")
    private String performerName;

    @Schema(description = "执行者核对标识", example = "1")
    private String performerCheckId;

    @Schema(description = "核对时间", example = "2024-01-15 10:00:00")
    private String checkTime;

    @Schema(description = "位置/科室标识", example = "1")
    private String locationId;

    @Schema(description = "位置/科室名称", example = "内科")
    private String locationName;

    @Schema(description = "机构标识", example = "1")
    private String orgId;

    @Schema(description = "原因文本", example = "感冒")
    private String reasonText;

    @Schema(description = "执行开始时间", example = "2024-01-15 10:30:00")
    private String occurrenceStartTime;

    @Schema(description = "执行结束时间", example = "2024-01-15 11:00:00")
    private String occurrenceEndTime;

    @Schema(description = "执行日期", example = "2024-01-15")
    private String occurrenceDate;

    @Schema(description = "内容JSON", example = "{\"dose\":\"500mg\"}")
    private String contentJson;

    @Schema(description = "医保分类枚举值", example = "MEDICAL_INSURANCE")
    private String ybClassEnum;

    @Schema(description = "打印次数", example = "1")
    private Integer printCount;

    @Schema(description = "备注", example = "注意事项")
    private String remark;

    @Schema(description = "嘱托内容", example = "饭后服用")
    private String entrustContent;

    @Schema(description = "执行次数", example = "1")
    private Integer executionCount;

    @Schema(description = "执行状态（0-未执行，1-已执行，2-已取消）", example = "1")
    private Integer executionStatus;

    @Schema(description = "执行状态名称", example = "已执行")
    private String executionStatusName;

    @Schema(description = "创建时间", example = "2024-01-15 09:00:00")
    private String createdAt;
}
