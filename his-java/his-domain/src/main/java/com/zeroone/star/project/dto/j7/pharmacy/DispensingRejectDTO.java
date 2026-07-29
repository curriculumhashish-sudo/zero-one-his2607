package com.zeroone.star.project.dto.j7.pharmacy;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;

/**
 * <p>
 * 描述：住院发药驳回数据传输对象
 * </p>
 * <p>
 * 功能说明：用于接收前端提交的住院发药驳回请求参数。
 * 包含患者信息、发药单据ID、汇总单ID和驳回原因。
 * 服务层根据该 DTO 更新 dispensing_header 表和 summary_order 表的状态。
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 01star
 * @version 1.0.0
 */
@Getter
@Setter
@AllArgsConstructor
@NoArgsConstructor
@Schema(description = "住院发药驳回DTO")
public class DispensingRejectDTO {
    @Schema(description = "患者ID", requiredMode = Schema.RequiredMode.REQUIRED)
    private String patientId;

    @Schema(description = "患者姓名")
    private String patientName;

    @Schema(description = "汇总单ID", requiredMode = Schema.RequiredMode.REQUIRED)
    private String summaryId;

    @Schema(description = "发药单据ID", requiredMode = Schema.RequiredMode.REQUIRED)
    private String dispensingId;

    @Schema(description = "驳回原因")
    private String rejectReason;
}
