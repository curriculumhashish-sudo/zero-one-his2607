package com.zeroone.star.project.dto.j6.pharmacy.stockout;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import jakarta.validation.Valid;
import jakarta.validation.constraints.NotEmpty;
import jakarta.validation.constraints.NotNull;
import java.math.BigDecimal;
import java.util.List;

@Data
@Schema(description = "出库单保存请求DTO")
public class StockOutSaveDTO {

    @Schema(description = "单据号（新增时为空，编辑时必填）", example = "")
    private String busNo;

    @Schema(description = "源药库ID", requiredMode = Schema.RequiredMode.REQUIRED, example = "284503862625177600")
    @NotNull(message = "源药库不能为空")
    private Long sourceLocationId;

    @Schema(description = "目的药房ID", requiredMode = Schema.RequiredMode.REQUIRED, example = "284503862625177700")
    @NotNull(message = "目的药房不能为空")
    private Long purposeLocationId;

    @Schema(description = "制单人员ID", requiredMode = Schema.RequiredMode.REQUIRED, example = "100001")
    @NotNull(message = "制单人员不能为空")
    private Long practitionerId;

    @Schema(description = "项目类型", example = "药品")
    private String projectType = "药品";

    @Schema(description = "关联请领单号", example = "")
    private String relatedReqNo;

    @Schema(description = "版本号（乐观锁，系统自动管理无需手动传）", example = "0")
    private Integer rowVersion;

    @Schema(description = "备注", example = "测试出库单")
    private String remark;

    @Schema(description = "出库明细行列表", requiredMode = Schema.RequiredMode.REQUIRED)
    @NotEmpty(message = "至少添加一条明细")
    @Valid
    private List<StockOutLineDTO> lines;

    @Data
    @Schema(description = "出库明细行DTO")
    public static class StockOutLineDTO {
        @Schema(description = "物品表名（med_medication_definition 或 adm_device_definition）", requiredMode = Schema.RequiredMode.REQUIRED, example = "med_medication_definition")
        private String itemTable;

        @Schema(description = "药品ID", requiredMode = Schema.RequiredMode.REQUIRED, example = "420000000000000001")
        @NotNull(message = "药品ID不能为空")
        private Long itemId;

        @Schema(description = "数量", requiredMode = Schema.RequiredMode.REQUIRED, example = "20")
        @NotNull(message = "数量不能为空")
        private BigDecimal itemQuantity;

        @Schema(description = "单位编码", requiredMode = Schema.RequiredMode.REQUIRED, example = "BOX")
        private String unitCode;

        @Schema(description = "批号", requiredMode = Schema.RequiredMode.REQUIRED, example = "LOT20260801")
        private String lotNumber;

        @Schema(description = "单价", requiredMode = Schema.RequiredMode.REQUIRED, example = "36.50")
        @NotNull(message = "单价不能为空")
        private BigDecimal price;
    }
}