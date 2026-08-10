package com.zeroone.star.project.dto.j7.pharmacy;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.NotEmpty;
import jakarta.validation.constraints.Size;
import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;

import java.util.List;

/**
 * <p>
 * 描述：住院退药确认数据传输对象
 * </p>
 * <p>
 * 功能说明：用于接收前端提交的住院退药确认请求参数。
 * 包含退药申请单ID、患者信息、勾选的药品明细ID列表和操作人。
 * 服务层根据该 DTO 更新 return_header 表状态，
 * 并根据勾选的药品列表回补 drug_stock 表库存。
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
@Schema(description = "住院退药确认DTO")
public class ReturnConfirmDTO {
    @NotBlank(message = "退药申请单ID不能为空")
    @Size(max = 64, message = "退药申请单ID长度不能超过64个字符")
    @Schema(description = "退药申请单ID", example = "990000000000000001", requiredMode = Schema.RequiredMode.REQUIRED)
    private String returnId;

    @NotBlank(message = "患者ID不能为空")
    @Size(max = 64, message = "患者ID长度不能超过64个字符")
    @Schema(description = "患者ID", example = "326940849401761792", requiredMode = Schema.RequiredMode.REQUIRED)
    private String patientId;

    @Schema(description = "患者姓名", example = "周女士")
    private String patientName;

    @NotEmpty(message = "确认退药的药品明细不能为空")
    @Schema(description = "确认退药的药品明细ID列表", example = "[\"990000000000000001\",\"990000000000000002\"]", requiredMode = Schema.RequiredMode.REQUIRED)
    private List<String> itemIds;

    @Schema(description = "操作人", example = "308791351298756608")
    private String operator;
}
