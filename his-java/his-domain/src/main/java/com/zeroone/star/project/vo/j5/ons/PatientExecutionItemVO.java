package com.zeroone.star.project.vo.j5.ons;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

@Data
@Schema(description = "患者执行项目")
public class PatientExecutionItemVO {

    @Schema(description = "记录ID", example = "1")
    private String id;

    @Schema(description = "患者ID", example = "1")
    private String patientId;

    @Schema(description = "患者姓名", example = "张三")
    private String patientName;

    @Schema(description = "就诊标识", example = "1")
    private String encounterId;

    @Schema(description = "项目名称", example = "血压测量")
    private String itemName;

    @Schema(description = "项目编码", example = "ITEM001")
    private String itemCode;

    @Schema(description = "项目类型", example = "检查")
    private String itemType;

    @Schema(description = "项目分类枚举", example = "MEDICATION")
    private String categoryEnum;

    @Schema(description = "数量", example = "1")
    private Integer quantity;

    @Schema(description = "单位", example = "次")
    private String unit;

    @Schema(description = "状态（0-未执行，1-已执行，2-已取消）", example = "0")
    private Integer status;

    @Schema(description = "状态名称", example = "待执行")
    private String statusName;

    @Schema(description = "优先级枚举", example = "ROUTINE")
    private String priorityEnum;

    @Schema(description = "优先级名称", example = "普通")
    private String priorityName;

    @Schema(description = "执行者ID", example = "1")
    private String performerId;

    @Schema(description = "执行者姓名", example = "护士小王")
    private String performerName;

    @Schema(description = "开单时间", example = "2024-01-15 09:00:00")
    private String orderTime;

    @Schema(description = "执行时间", example = "2024-01-15 10:00:00")
    private String executeTime;

    @Schema(description = "备注", example = "注意事项")
    private String remark;
}
