package com.zeroone.star.project.vo.j6.pharmacy.inventory;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.math.BigDecimal;
import java.time.LocalDate;

@Data
@Schema(description = "库存明细列表单行数据")
public class InventoryDetailVO {
    @Schema(description = "主键ID", example = "323019059251646464")
    private String id;

    @Schema(description = "商品主表标识", example = "med_medication_definition")
    private String itemTable;

    @Schema(description = "商品ID", example = "322994478289784832")
    private String itemId;

    @Schema(description = "收费项目定义ID", example = "322994478293979136")
    private String chargeItemDefinitionId;

    @Schema(description = "产品业务编号", example = "MD0022")
    private String productBusNo;

    @Schema(description = "通用名称", example = "替米沙坦片")
    private String name;

    @Schema(description = "商品名", example = "嘉瑟宜")
    private String merchandiseName;

    @Schema(description = "库存总数量", example = "100.00")
    private BigDecimal totalVolume;

    @Schema(description = "生产厂家", example = "上海信谊天平药业有限公司")
    private String manufacturer;

    @Schema(description = "规格", example = "40mg")
    private String specification;

    @Schema(description = "批准文号", example = "国药准字H44021351")
    private String approvalNumber;

    @Schema(description = "医保编码", example = "XA001001")
    private String ybNo;

    @Schema(description = "追溯码", example = "TR2026080100001")
    private String traceNo;

    @Schema(description = "拼音简码", example = "TMSTP")
    private String pyStr;

    @Schema(description = "五笔简码", example = "FOIFT")
    private String wbStr;

    @Schema(description = "批次号", example = "001-002")
    private String lotNumber;

    @Schema(description = "仓库ID", example = "322954885380706304")
    private String warehouseId;

    @Schema(description = "仓库名称", example = "西药库")
    private String warehouseName;

    @Schema(description = "供应商ID", example = "4100000000000000004")
    private String supplierId;

    @Schema(description = "供应商名称", example = "华中药品配送有限公司")
    private String supplierName;

    @Schema(description = "生产日期")
    private LocalDate productionDate;

    @Schema(description = "有效期至")
    private LocalDate expirationDate;

    @Schema(description = "剩余有效天数", example = "180")
    private Integer remainingDays;

    @Schema(description = "库存状态编码 NORMAL正常 STOP_SUPPLY停供", example = "ACTIVE")
    private String inventoryStatusEnum;

    @Schema(description = "库存状态展示文字", example = "正常")
    private String inventoryStatusLabel;

    @Schema(description = "医保等级编码", example = "1")
    private String chrgitmLv;

    @Schema(description = "医保等级名称", example = "甲类")
    private String chrgitmLvLabel;

    @Schema(description = "药品分类编码", example = "2")
    private String medicationCategoryCode;

    @Schema(description = "器械分类编码", example = "")
    private String deviceCategoryCode;

    @Schema(description = "分类展示名称", example = "2")
    private String categoryLabel;

    @Schema(description = "剂型编码", example = "2")
    private String doseFormCode;

    @Schema(description = "剂型名称", example = "2")
    private String doseFormLabel;

    @Schema(description = "包装单位编码", example = "1")
    private String packageUnitCode;

    @Schema(description = "大包装单位", example = "1")
    private String packageUnitName;

    @Schema(description = "拆零单位", example = "4")
    private String splitUnitName;

    @Schema(description = "包装换算比例", example = "4")
    private BigDecimal packageUnitRatio;

    @Schema(description = "包装库存文本展示", example = "25盒")
    private String packageStockText;

    @Schema(description = "拆零库存文本展示", example = "100片")
    private String splitStockText;

    @Schema(description = "批次进价", example = "13.36")
    private BigDecimal lotCostPrice;

    @Schema(description = "批次售价", example = "13.36")
    private BigDecimal lotSalePrice;

    @Schema(description = "当前批次总成本金额", example = "1336.00")
    private BigDecimal lineCostTotal;

    @Schema(description = "当前批次总销售金额", example = "1336.00")
    private BigDecimal linePriceTotal;
}
