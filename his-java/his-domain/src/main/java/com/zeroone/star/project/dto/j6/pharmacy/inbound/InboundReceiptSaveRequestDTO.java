package com.zeroone.star.project.dto.j6.pharmacy.inbound;

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
 * 药库进货单保存/保存并提交请求对象。
 * 结构与新增单据返回响应（InboundReceiptDetailVO）一致，
 * 前端编辑后原样回传。
 */
@Data
@Schema(name = "InboundReceiptSaveRequestDTO", description = "药库进货单保存/保存并提交请求")
public class InboundReceiptSaveRequestDTO {

    @Schema(description = "业务单号（新增时为空，保存已有单据时必传）", example = "CIR202608070001")
    private String busNo;

    @Schema(description = "来源业务单号", example = "DHD202607180001")
    private String originalBusNo;

    @NotBlank(message = "供应类型不能为空")
    @Schema(description = "供应类型编码（进货单固定值）", example = "CABINET_INBOUND_RECEIPT")
    private String typeEnum;

    @NotBlank(message = "供应类别不能为空")
    @Schema(description = "供应类别编码", example = "PURCHASE")
    private String categoryEnum;

    @Schema(description = "供应状态编码", example = "DRAFT")
    private String statusEnum;

    @Schema(description = "来源位置类型", example = "SUPPLIER")
    private String sourceTypeEnum;

    @Schema(description = "来源位置标识")
    private String sourceLocationId;

    @NotBlank(message = "目的位置类型不能为空")
    @Schema(description = "目的位置类型", example = "WAREHOUSE")
    private String purposeTypeEnum;

    @NotBlank(message = "目的库房不能为空")
    @Schema(description = "目的库房标识", example = "322954885380706304")
    private String purposeLocationId;

    @Schema(description = "库房名称（展示用，后端忽略）", example = "西药库")
    private String purposeLocationName;

    @NotBlank(message = "供应商不能为空")
    @Schema(description = "供应商标识", example = "4100000000000000004")
    private String supplierId;

    @Schema(description = "供应商名称（展示用，后端忽略）", example = "华中药品配送有限公司")
    private String supplierName;

    @Schema(description = "经办人ID", example = "322932096108597248")
    private String practitionerId;

    @Schema(description = "制单人ID", example = "322932096108597248")
    private String creatorId;

    @Schema(description = "制单人名称（展示用，后端忽略）", example = "库管")
    private String creatorName;

    @Schema(description = "申请人ID", example = "322932096108597248")
    private String applicantId;

    @Schema(description = "申请人名称（展示用，后端忽略）", example = "库管")
    private String applicantName;

    @Schema(description = "审批人ID", example = "308791351298756608")
    private String approverId;

    @Schema(description = "审批人名称（展示用，后端忽略）", example = "admin")
    private String approverName;

    @Schema(description = "申请时间（提交审批时服务端写入）")
    private LocalDateTime applyTime;

    @Schema(description = "审批时间（审批时服务端写入）")
    private LocalDateTime approvalTime;

    @Schema(description = "单据总金额（展示用，后端忽略）", example = "1090.00")
    private BigDecimal totalAmount;

    @Schema(description = "原因/备注", example = "按药库订货单到货入库")
    private String reason;

    @Schema(description = "乐观锁版本号（保存已有单据时必传）", example = "0")
    private Integer version;

    @Valid
    @NotEmpty(message = "进货明细不能为空")
    @Schema(description = "进货明细行列表")
    private List<InboundReceiptLineDTO> lines;

    /**
     * 药库进货单明细行。
     */
    @Data
    @Schema(name = "InboundReceiptLineDTO", description = "药库进货单明细行")
    public static class InboundReceiptLineDTO {

        @NotBlank(message = "项目物理表不能为空")
        @Schema(description = "项目物理表（药品=med_medication_definition，耗材=adm_device_definition）",
                example = "med_medication_definition")
        private String itemTable;

        @NotBlank(message = "项目标识不能为空")
        @Schema(description = "项目定义主键（药品/耗材ID）", example = "322994478289784832")
        private String itemId;

        @NotNull(message = "进货数量不能为空")
        @Schema(description = "行数量（拆零最小单位）", example = "200.00")
        private BigDecimal itemQuantity;

        @NotBlank(message = "单位编码不能为空")
        @Schema(description = "单位编码", example = "1")
        private String unitCode;

        @Schema(description = "项目名称（展示用，后端忽略）", example = "替米沙坦片")
        private String productName;

        @Schema(description = "规格（展示用，后端忽略）", example = "40mg")
        private String specification;

        @Schema(description = "厂家（展示用，后端忽略）", example = "上海信谊天平药业有限公司")
        private String manufacturer;

        @Schema(description = "批号", example = "001-002")
        private String lotNumber;

        @Schema(description = "追溯码", example = "TR2026080100001")
        private String traceNo;

        @Schema(description = "单价（包装整包进价）", example = "13.36")
        private BigDecimal price;

        @Schema(description = "行合计金额（数量×单价）", example = "2672.00")
        private BigDecimal totalPrice;

        @Schema(description = "发生时间（不传默认当前时间）", example = "2026-08-07T10:30:00")
        private LocalDateTime occurrenceTime;

        @Schema(description = "效期区间起点（生产日期）", example = "2026-06-01T00:00:00")
        private LocalDateTime startTime;

        @Schema(description = "效期区间止点（有效期至）", example = "2027-07-31T00:00:00")
        private LocalDateTime endTime;

        @Schema(description = "行备注", example = "首批到货")
        private String remake;
    }
}
