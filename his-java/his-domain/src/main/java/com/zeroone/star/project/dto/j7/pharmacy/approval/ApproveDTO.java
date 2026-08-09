package com.zeroone.star.project.dto.j7.pharmacy.approval;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.Size;
import lombok.Data;

import java.time.LocalDateTime;

@Schema(description = "审批DTO")
@Data
public class ApproveDTO {

    @Schema(description = "盘点单明细编号", example = "PST20260630120000001")
    @NotBlank(message = "盘点单明细编号不能为空")
    @Size(max = 255, message = "盘点单明细编号长度不能超过255个字符")
    private String busNo;

    @Schema(description = "审批人标识", example = "308791351298756608")
    @NotBlank(message = "审批人标识不能为空")
    @Size(max = 64, message = "审批人标识长度不能超过64个字符")
    private String approverId;

    @Size(max = 255, message = "备注长度不能超过255个字符")
    @Schema(description = "备注(驳回需要有备注)", example = "库存数据有误，请核实后重新提交")
    private String remark;

    @Schema(description = "审批时间；不传时由后端使用当前时间", example = "2026-08-08 10:00:00")
    private LocalDateTime approvalTime;
}
