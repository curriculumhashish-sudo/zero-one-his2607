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

    @Schema(description = "单据号（新增时为空，编辑时必填）")
    private String busNo;

    @Schema(description = "源药库ID", required = true)
    @NotNull(message = "源药库不能为空")
    private Long sourceLocationId;

    @Schema(description = "目的药房ID", required = true)
    @NotNull(message = "目的药房不能为空")
    private Long purposeLocationId;

    @Schema(description = "制单人员ID", required = true)
    @NotNull(message = "制单人员不能为空")
    private Long practitionerId;

    @Schema(description = "项目类型", example = "药品")
    private String projectType = "药品";

    @Schema(description = "关联请领单号")
    private String relatedReqNo;

    @Schema(description = "版本号（乐观锁）")
    private Integer rowVersion;

    @Schema(description = "备注")
    private String remark;

    @Schema(description = "出库明细行列表", required = true)
    @NotEmpty(message = "至少添加一条明细")
    @Valid
    private List<StockOutLineDTO> lines;

    @Data
    @Schema(description = "出库明细行DTO")
    public static class StockOutLineDTO {
        @Schema(description = "物品表名", required = true)
        private String itemTable;

        @Schema(description = "药品ID", required = true)
        @NotNull(message = "药品ID不能为空")
        private Long itemId;

        @Schema(description = "数量", required = true)
        @NotNull(message = "数量不能为空")
        private BigDecimal itemQuantity;

        @Schema(description = "单位编码", required = true)
        private String unitCode;

        @Schema(description = "批号", required = true)
        private String lotNumber;

        @Schema(description = "单价", required = true)
        @NotNull(message = "单价不能为空")
        private BigDecimal price;
    }
}