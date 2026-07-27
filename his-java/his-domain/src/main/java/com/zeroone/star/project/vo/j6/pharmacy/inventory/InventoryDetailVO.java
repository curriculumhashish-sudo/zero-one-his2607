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

    @Schema(description = "商品主表标识", example = "medication")
    private String itemTable;

    @Schema(description = "商品ID", example = "987654321001")
    private String itemId;

    @Schema(description = "收费项目定义ID", example = "CH001234")
    private String chargeItemDefinitionId;

    @Schema(description = "产品业务编号", example = "BUS202607190001")
    private String productBusNo;

    @Schema(description = "通用名称", example = "阿莫西林胶囊")
    private String name;

    @Schema(description = "商品名", example = "阿莫仙")
    private String merchandiseName;

    @Schema(description = "库存总数量", example = "1000")
    private BigDecimal totalVolume;

    @Schema(description = "生产厂家", example = "珠海联邦制药股份有限公司")
    private String manufacturer;

    @Schema(description = "批准文号", example = "国药准字H44021351")
    private String approvalNumber;

    @Schema(description = "医保编码", example = "XA001001")
    private String ybNo;

    @Schema(description = "追溯码", example = "TR2026060100001")
    private String traceNo;

    @Schema(description = "拼音简码", example = "amxljn")
    private String pyStr;

    @Schema(description = "五笔简码", example = "gfsf")
    private String wbStr;

    @Schema(description = "批次号", example = "20260512")
    private String lotNumber;

    @Schema(description = "仓库ID", example = "WH001")
    private String warehouseId;

    @Schema(description = "仓库名称", example = "西药一号仓库")
    private String warehouseName;

    @Schema(description = "供应商ID", example = "SUP007")
    private String supplierId;

    @Schema(description = "供应商名称", example = "广州医药有限公司")
    private String supplierName;

    @Schema(description = "生产日期")
    private LocalDate productionDate;

    @Schema(description = "有效期至")
    private LocalDate expirationDate;

    @Schema(description = "剩余有效天数", example = "180")
    private Integer remainingDays;

    @Schema(description = "库存状态编码枚举 NORMAL正常 STOP_SUPPLY停供", example = "NORMAL")
    private String inventoryStatusEnum;

    @Schema(description = "库存状态展示文字", example = "正常供货")
    private String inventoryStatusLabel;

    @Schema(description = "院内等级编码", example = "A")
    private String chrgitmLv;

    @Schema(description = "院内等级名称", example = "甲类重点药品")
    private String chrgitmLvLabel;

    @Schema(description = "药品分类编码", example = "0101")
    private String medicationCategoryCode;

    @Schema(description = "器械分类编码", example = "")
    private String deviceCategoryCode;

    @Schema(description = "分类展示名称", example = "抗感染口服西药")
    private String categoryLabel;

    @Schema(description = "剂型编码", example = "JN")
    private String doseFormCode;

    @Schema(description = "剂型名称", example = "胶囊剂")
    private String doseFormLabel;

    @Schema(description = "包装单位编码", example = "HE")
    private String packageUnitCode;

    @Schema(description = "大包装单位", example = "盒")
    private String packageUnitName;

    @Schema(description = "拆零单位", example = "粒")
    private String splitUnitName;

    @Schema(description = "包装换算比例", example = "24")
    private BigDecimal packageUnitRatio;

    @Schema(description = "包装库存文本展示", example = "41盒")
    private String packageStockText;

    @Schema(description = "拆零库存文本展示", example = "984粒")
    private String splitStockText;

    @Schema(description = "批次进价", example = "12.50")
    private BigDecimal lotCostPrice;

    @Schema(description = "批次售价", example = "18.20")
    private BigDecimal lotSalePrice;

    @Schema(description = "当前批次总成本金额", example = "512.50")
    private BigDecimal lineCostTotal;

    @Schema(description = "当前批次总销售金额", example = "746.20")
    private BigDecimal linePriceTotal;
}
