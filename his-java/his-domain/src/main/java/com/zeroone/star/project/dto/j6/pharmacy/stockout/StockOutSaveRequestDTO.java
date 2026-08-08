package com.zeroone.star.project.dto.j6.pharmacy.stockout;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.Valid;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.NotEmpty;
import jakarta.validation.constraints.NotNull;
import lombok.Data;

import java.math.BigDecimal;
import java.time.LocalDateTime;
import java.util.List;

/**
 * 药库出库单保存/保存并提交请求对象。
 * 结构与新增单据返回响应一致，前端编辑后原样回传。
 */
@Data
@Schema(name = "StockOutSaveRequestDTO", description = "药库出库单保存/保存并提交请求")
public class StockOutSaveRequestDTO {

    @Schema(description = "业务单号（新增时为空，保存已有单据时必传）", example = "CSO202608070001")
    private String busNo;

    @Schema(description = "关联请领单号", example = "QL202608070001")
    private String originalBusNo;

    @NotBlank(message = "类型不能为空")
    @Schema(description = "出库单类型编码", example = "WAREHOUSE_STOCKOUT")
    private String typeEnum;

    @NotBlank(message = "类别不能为空")
    @Schema(description = "供应类别编码", example = "STOCK_OUT")
    private String categoryEnum;

    @Schema(description = "供应状态编码", example = "PENDING_APPROVAL")
    private String statusEnum;

    @NotBlank(message = "源类型不能为空")
    @Schema(description = "来源位置类型", example = "WAREHOUSE")
    private String sourceTypeEnum;

    @NotBlank(message = "源库房不能为空")
    @Schema(description = "源药库ID", example = "322954885380706304")
    private String sourceLocationId;

    @Schema(description = "源药库名称（展示用，后端忽略）", example = "西药库")
    private String sourceLocationName;

    @NotBlank(message = "目的类型不能为空")
    @Schema(description = "目的位置类型", example = "PHARMACY")
    private String purposeTypeEnum;

    @NotBlank(message = "目的药房不能为空")
    @Schema(description = "目的药房ID", example = "322954949549363200")
    private String purposeLocationId;

    @Schema(description = "目的药房名称（展示用，后端忽略）", example = "门诊西药房")
    private String purposeLocationName;

    @Schema(description = "制单人员ID", example = "322932096108597248")
    private String practitionerId;

    @Schema(description = "制单人名称（展示用，后端忽略）", example = "库管")
    private String practitionerName;

    @Schema(description = "创建人ID")
    private String creatorId;

    @Schema(description = "创建人名称（展示用，后端忽略）", example = "库管")
    private String creatorName;

    @Schema(description = "申请人ID")
    private String applicantId;

    @Schema(description = "申请人名称（展示用，后端忽略）", example = "库管")
    private String applicantName;

    @Schema(description = "审批人ID")
    private String approverId;

    @Schema(description = "审批人名称（展示用，后端忽略）", example = "admin")
    private String approverName;

    @Schema(description = "制单/申请时间")
    private LocalDateTime applyTime;

    @Schema(description = "审批时间")
    private LocalDateTime approvalTime;

    @Schema(description = "单据总金额（展示用，后端忽略）", example = "1336.00")
    private BigDecimal totalAmount;

    @Schema(description = "备注", example = "门诊药房调拨出库")
    private String remark;

    @Schema(description = "乐观锁版本号（新增时为0，保存已有单据时传上次返回值）", example = "0")
    private Integer version;

    @Valid
    @NotEmpty(message = "出库明细不能为空")
    @Schema(description = "出库明细行列表")
    private List<StockOutLineDTO> lines;

    /**
     * 药库出库单明细行。
     */
    @Data
    @Schema(name = "StockOutLineDTO", description = "药库出库单明细行")
    public static class StockOutLineDTO {

        @NotBlank(message = "项目物理表不能为空")
        @Schema(description = "项目物理表（药品=med_medication_definition，耗材=adm_device_definition）",
                example = "med_medication_definition")
        private String itemTable;

        @NotBlank(message = "项目标识不能为空")
        @Schema(description = "项目定义主键（药品/耗材ID）", example = "322994478289784832")
        private String itemId;

        @NotNull(message = "出库数量不能为空")
        @Schema(description = "行数量（拆零最小单位）", example = "100.00")
        private BigDecimal itemQuantity;

        @NotBlank(message = "单位编码不能为空")
        @Schema(description = "单位编码", example = "1")
        private String unitCode;

        @Schema(description = "项目名称（展示用，后端忽略）", example = "替米沙坦片")
        private String productName;

        @Schema(description = "规格（展示用，后端忽略）", example = "40mg")
        private String specification;

        @Schema(description = "厂家/产地（展示用，后端忽略）", example = "上海信谊天平药业有限公司")
        private String manufacturer;

        @Schema(description = "批号（必须与源库 wkf_inventory_item.lot_number 一致）", example = "001-002")
        private String lotNumber;

        @Schema(description = "追溯码", example = "TR2026080100001")
        private String traceNo;

        @Schema(description = "单价", example = "13.36")
        private BigDecimal price;

        @Schema(description = "行合计金额（数量×单价）", example = "1336.00")
        private BigDecimal totalPrice;

        @Schema(description = "发生时间（不传默认当前时间）", example = "2026-08-07T10:30:00")
        private LocalDateTime occurrenceTime;

        @Schema(description = "效期区间起点（生产日期）", example = "2026-06-01T00:00:00")
        private LocalDateTime startTime;

        @Schema(description = "效期区间止点（有效期至）", example = "2027-07-31T00:00:00")
        private LocalDateTime endTime;

        @Schema(description = "行备注", example = "首批调拨")
        private String remake;
    }
}
