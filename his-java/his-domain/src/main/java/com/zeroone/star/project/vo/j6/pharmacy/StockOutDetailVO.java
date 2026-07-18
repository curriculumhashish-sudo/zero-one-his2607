package com.zeroone.star.project.vo.j6.pharmacy;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.math.BigDecimal;
import java.time.LocalDateTime;
import java.util.List;

/**
 * <p>
 * 描述：出库单详情视图对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j6小组-告别
 * @version 1.0.0
 */
@Data
@Schema(description = "出库单详情")
public class StockOutDetailVO {
    // ==================== 单据头 ====================

    /**
     * 业务单号
     */
    @Schema(description = "业务单号", example = "SO202607180001")
    private String busNo;

    /**
     * 来源业务单号
     */
    @Schema(description = "来源业务单号", example = "PO202607150001")
    private String originalBusNo;

    /**
     * 供应类型
     */
    @Schema(description = "供应类型", example = "CABINET_STOCK_OUT")
    private String typeEnum;

    /**
     * 审批状态
     */
    @Schema(description = "审批状态", example = "APPROVAL")
    private String statusEnum;

    /**
     * 来源库房ID
     */
    @Schema(description = "出库药库ID", example = "310000000000000201")
    private String sourceLocationId;

    /**
     * 来源库房名称
     */
    @Schema(description = "出库药库名称", example = "中心药库")
    private String sourceLocationName;

    /**
     * 目的库房ID
     */
    @Schema(description = "目的药房ID", example = "310000000000000301")
    private String purposeLocationId;

    /**
     * 目的库房名称
     */
    @Schema(description = "目的药房名称", example = "门诊药房")
    private String purposeLocationName;

    /**
     * 来源类型（双端点单据-出方）
     */
    @Schema(description = "来源类型", example = "WAREHOUSE")
    private String sourceTypeEnum;

    /**
     * 目的类型（双端点单据-进方）
     */
    @Schema(description = "目的类型", example = "PHARMACY")
    private String purposeTypeEnum;

    /**
     * 供应类别
     */
    @Schema(description = "供应类别", example = "STOCK_OUT")
    private String categoryEnum;

    /**
     * 行版本号（乐观锁）
     */
    @Schema(description = "行版本号", example = "0")
    private Integer rowVersion;

    /**
     * 制单人ID
     */
    @Schema(description = "制单人ID", example = "310000000000000101")
    private String applicantId;

    /**
     * 制单人姓名
     */
    @Schema(description = "制单人姓名", example = "张三")
    private String applicantName;

    /**
     * 申请时间
     */
    @Schema(description = "申请时间", example = "2026-07-18 09:00:00")
    private LocalDateTime applyTime;

    /**
     * 审批人ID
     */
    @Schema(description = "审批人ID", example = "310000000000000102")
    private String approverId;

    /**
     * 审批人姓名
     */
    @Schema(description = "审批人姓名", example = "李四")
    private String approverName;

    /**
     * 审批时间
     */
    @Schema(description = "审批时间", example = "2026-07-18 10:30:00")
    private LocalDateTime approvalTime;

    /**
     * 驳回原因
     */
    @Schema(description = "驳回原因", example = "药品库存不足")
    private String reason;

    // ==================== 明细行 ====================

    /**
     * 出库单明细行列表
     */
    @Schema(description = "明细行列表")
    private List<StockOutDetailLineVO> lines;

    /**
     * <p>
     * 描述：出库单详情明细行
     * </p>
     */
    @Data
    @Schema(description = "出库单详情明细行")
    public static class StockOutDetailLineVO {
        /**
         * 主键
         */
        @Schema(description = "明细行ID", example = "310000000000000701")
        private String id;

        /**
         * 物品定义表名
         */
        @Schema(description = "物品定义表名", example = "med_medication_definition")
        private String itemTable;

        /**
         * 物品ID
         */
        @Schema(description = "物品ID", example = "310000000000000401")
        private String itemId;

        /**
         * 出库数量
         */
        @Schema(description = "出库数量", example = "100.00")
        private BigDecimal itemQuantity;

        /**
         * 计量单位编码
         */
        @Schema(description = "计量单位编码", example = "BOX")
        private String unitCode;

        /**
         * 计量单位字典文本
         */
        @Schema(description = "计量单位名称", example = "盒")
        private String unitCodeDictText;

        /**
         * 发生时间
         */
        @Schema(description = "发生时间", example = "2026-07-18 09:00:00")
        private LocalDateTime occurrenceTime;

        /**
         * 原因
         */
        @Schema(description = "原因", example = "科室申领")
        private String reason;

        /**
         * 备注
         */
        @Schema(description = "备注", example = "加急出库")
        private String remake;

        /**
         * 产品名称
         */
        @Schema(description = "产品名称", example = "阿莫西林胶囊")
        private String productName;

        /**
         * 规格
         */
        @Schema(description = "规格", example = "0.25g*24粒")
        private String specification;

        /**
         * 生产厂家
         */
        @Schema(description = "生产厂家", example = "华北制药")
        private String manufacturer;

        /**
         * 批号
         */
        @Schema(description = "批号", example = "B202607001")
        private String lotNumber;

        /**
         * 追溯码
         */
        @Schema(description = "追溯码", example = "TRC202607180001")
        private String traceNo;

        /**
         * 单价
         */
        @Schema(description = "单价", example = "15.50")
        private BigDecimal price;

        /**
         * 合计金额
         */
        @Schema(description = "合计金额", example = "1550.00")
        private BigDecimal totalPrice;

        /**
         * 效期开始时间
         */
        @Schema(description = "效期开始时间", example = "2026-01-01 00:00:00")
        private LocalDateTime startTime;

        /**
         * 效期结束时间
         */
        @Schema(description = "效期结束时间", example = "2028-01-01 00:00:00")
        private LocalDateTime endTime;
    }
}
