package com.zeroone.star.project.dto.pharmacy;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;

import java.util.List;

/**
 * <p>
 * 描述：门诊拒绝退药数据传输对象
 * </p>
 * <p>
 * 功能说明：用于接收前端提交的门诊拒绝退药请求参数。
 * 包含退药申请单ID、患者信息、拒绝原因和驳回的药品明细ID列表。
 * 服务层根据该 DTO 更新 return_header 表状态为"已拒绝"。
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
@Schema(description = "门诊拒绝退药DTO")
public class ReturnRejectDTO {
    @Schema(description = "退药申请单ID", requiredMode = Schema.RequiredMode.REQUIRED)
    private String returnId;

    @Schema(description = "患者ID", requiredMode = Schema.RequiredMode.REQUIRED)
    private String patientId;

    @Schema(description = "患者姓名")
    private String patientName;

    @Schema(description = "驳回原因")
    private String rejectReason;

    @Schema(description = "驳回的药品明细ID列表")
    private List<String> itemIds;
}
