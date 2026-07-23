package com.zeroone.star.project.dto.j7.pharmacy;

import io.swagger.v3.oas.annotations.media.Schema;
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
    @Schema(description = "退药申请单ID", requiredMode = Schema.RequiredMode.REQUIRED)
    private String returnId;

    @Schema(description = "患者ID", requiredMode = Schema.RequiredMode.REQUIRED)
    private String patientId;

    @Schema(description = "患者姓名")
    private String patientName;

    @Schema(description = "确认退药的药品明细ID列表", requiredMode = Schema.RequiredMode.REQUIRED)
    private List<String> itemIds;

    @Schema(description = "操作人")
    private String operator;
}
