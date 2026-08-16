package com.zeroone.star.project.dto.stocktake;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.Valid;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.NotEmpty;
import jakarta.validation.constraints.Size;
import lombok.Data;

import java.math.BigDecimal;
import java.time.LocalDateTime;
import java.util.List;

/**
 * <p>
 * 描述：盘点单保存参数
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 北海
 * @version 1.0.0
 */
@Data
@Schema(description = "盘点单保存参数")
public class StocktakeSaveDTO {

    @Size(max = 255, message = "业务单号长度不能超过255个字符")
    @Schema(description = "业务单号（新增时可为空）", example = "PST202607300001")
    private String busNo;

    @Schema(description = "单据ID", example = "329874660147597312")
    private String id;

    @Schema(description = "单据状态", example = "DRAFT")
    private String statusEnum;

    @Schema(description = "制单人", example = "张三")
    private String createdByName;

    @Schema(description = "制单时间", example = "2026-07-30 10:00:00")
    private LocalDateTime createdAt;

    @Schema(description = "审批人", example = "李四")
    private String approverName;

    @Schema(description = "审批时间", example = "2026-07-30 14:00:00")
    private LocalDateTime approvalTime;

    @Schema(description = "租户标识", example = "1")
    private String tenantId;

    @NotBlank(message = "库房标识不能为空")
    @Size(max = 64, message = "库房标识长度不能超过64个字符")
    @Schema(description = "盘点药房", example = "322954949549363200", requiredMode = Schema.RequiredMode.REQUIRED)
    private String warehouseId;

    @Schema(description = "盘点发生时间", example = "2026-07-30 10:00:00")
    private LocalDateTime occurrenceTime;

    @Schema(description = "主表备注", example = "月度盘点")
    private String remark;

    @Schema(description = "盘点类型编码", example = "PHARMACY")
    private String purposeTypeEnum;

    @Schema(description = "申请人标识（未登录场景可回填）", example = "308791351298756608")
    private String applicantId;

    @Schema(description = "总金额", example = "1187.50")
    private BigDecimal totalPrice;

    @NotEmpty(message = "盘点明细不能为空")
    @Valid
    @Schema(description = "盘点明细列表", requiredMode = Schema.RequiredMode.REQUIRED)
    private List<StocktakeItemSaveDTO> items;
}
