package com.zeroone.star.project.dto.j7.dispense;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.Size;
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
public class DispenseRejectDTO {
    @NotBlank(message = "患者ID不能为空")
    @Size(max = 64, message = "患者ID长度不能超过64个字符")
    @Schema(description = "患者ID", example = "326940849401761792", requiredMode = Schema.RequiredMode.REQUIRED)
    private String patientId;

    @Size(max = 64, message = "患者姓名长度不能超过64个字符")
    @Schema(description = "患者姓名", example = "张三")
    private String patientName;

    @NotBlank(message = "汇总单ID不能为空")
    @Size(max = 64, message = "汇总单ID长度不能超过64个字符")
    @Schema(description = "汇总单ID", example = "SUM20260719001", requiredMode = Schema.RequiredMode.REQUIRED)
    private String summaryId;

    @NotBlank(message = "发药单据ID不能为空")
    @Size(max = 64, message = "发药单据ID长度不能超过64个字符")
    @Schema(description = "发药单据ID", example = "d12345678901234567890", requiredMode = Schema.RequiredMode.REQUIRED)
    private String dispensingId;

    @Size(max = 255, message = "驳回原因长度不能超过255个字符")
    @Schema(description = "驳回原因", example = "药品库存不足，驳回")
    private String rejectReason;
}
