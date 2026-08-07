package com.zeroone.star.project.dto.j7.pharmacy;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.Size;
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
    @NotBlank(message = "退药申请单ID不能为空")
    @Size(max = 64, message = "退药申请单ID长度不能超过64个字符")
    @Schema(description = "退药申请单ID", example = "326940849401761792", requiredMode = Schema.RequiredMode.REQUIRED)
    private String returnId;

    @NotBlank(message = "患者ID不能为空")
    @Size(max = 64, message = "患者ID长度不能超过64个字符")
    @Schema(description = "患者ID", example = "326940849401761792", requiredMode = Schema.RequiredMode.REQUIRED)
    private String patientId;

    @Schema(description = "患者姓名", example = "张三")
    private String patientName;

    @Size(max = 255, message = "驳回原因长度不能超过255个字符")
    @Schema(description = "驳回原因", example = "药品已拆封，不符合退药条件")
    private String rejectReason;

    @Schema(description = "驳回的药品明细ID列表", example = "[\"329891909520592897\"]")
    private List<String> itemIds;
}
