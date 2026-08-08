package com.zeroone.star.project.dto.j8.mzfymx;

import org.apache.fesod.sheet.annotation.ExcelProperty;
import org.apache.fesod.sheet.annotation.write.style.ColumnWidth;
import org.apache.fesod.sheet.annotation.write.style.ContentRowHeight;
import org.apache.fesod.sheet.annotation.write.style.HeadRowHeight;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.math.BigDecimal;

/**
 * <p>
 * 描述：门诊费用明细-导出 Excel 单行数据 DTO（对应导出接口 MinXiApis#exportMinXi）
 *      ⚠️ 与查询接口 MinXiDTO 的字段对应关系：
 *      相同点：列表所有展示列 + 金额列完全一致（Excel 列顺序和页面列表顺序对齐）
 *      不同点：不含内部主键追踪字段（chargeItemId/paymentId/orderingDoctorId/cashierId）、
 *              不含分页序号 sequenceNo、不含分页 pageIndex/pageSize 入参。
 *      导出逻辑：复用 MinXiService 的查询 SQL（不带 LIMIT 分页，全量导出），
 *              结果集 List&#60;MinXiDTO&#62; 直接 MapStruct 映射为 List&#60;MinXiExportDTO&#62; 后写 Excel。
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 阿伟学长
 * @version 1.0.0
 */
@Data
@HeadRowHeight(20)
@ContentRowHeight(18)
@Schema(description = "门诊费用明细-导出Excel单行数据")
public class MinXiExportDTO {

    // ============================================
    // 📋 Excel 列表列（顺序与原型图一致，EasyExcel @ExcelProperty 按此顺序生成 Sheet1 列）
    // ============================================
    @ExcelProperty("门诊号")
    @ColumnWidth(22)
    @Schema(description = "门诊号（就诊号，来源 adm_encounter.bus_no）")
    private String outpatientNo;

    @ExcelProperty("患者")
    @ColumnWidth(12)
    @Schema(description = "患者姓名（来源 adm_patient.name）")
    private String patientName;

    @ExcelProperty("挂号科室")
    @ColumnWidth(14)
    @Schema(description = "挂号科室（来源 adm_organization.name）")
    private String deptName;

    @ExcelProperty("就诊日期")
    @ColumnWidth(14)
    @Schema(description = "就诊日期（格式 yyyy-MM-dd，来源 adm_encounter.start_time）")
    private String encounterDate;

    @ExcelProperty("大类")
    @ColumnWidth(10)
    @Schema(description = "费用大类展示文案（如 挂号/药品/耗材/医疗服务）")
    private String feeCategoryDisplay;

    @ExcelProperty("细分类")
    @ColumnWidth(12)
    @Schema(description = "细分类展示文案（暂无细分类字典，暂与费用大类一致）")
    private String subCategoryDisplay;

    @ExcelProperty("项目名称")
    @ColumnWidth(20)
    @Schema(description = "项目名称（wkf/med/dev 3 张产品表 name + 挂号类 def.charge_name 兜底）")
    private String productName;

    @ExcelProperty("院内编码")
    @ColumnWidth(14)
    @Schema(description = "院内项目编码（3 张产品表 bus_no）")
    private String productCode;

    @ExcelProperty("规格")
    @ColumnWidth(12)
    @Schema(description = "规格（当前产品表无该字段，暂返回空，后续补充）")
    private String specification;

    @ExcelProperty("数量")
    @ColumnWidth(10)
    @Schema(description = "数量")
    private BigDecimal quantityValue;

    @ExcelProperty("单位")
    @ColumnWidth(8)
    @Schema(description = "数量单位")
    private String quantityUnitDisplay;

    @ExcelProperty("单价")
    @ColumnWidth(12)
    @Schema(description = "单价")
    private BigDecimal unitPrice;

    @ExcelProperty("开单医生")
    @ColumnWidth(12)
    @Schema(description = "开单医生姓名")
    private String orderingDoctorName;

    @ExcelProperty("收费人")
    @ColumnWidth(12)
    @Schema(description = "收费人姓名")
    private String cashierName;

    @ExcelProperty("结算时间")
    @ColumnWidth(22)
    @Schema(description = "结算时间（格式：yyyy-MM-dd HH:mm:ss）")
    private String billDate;

    @ExcelProperty("收退方向")
    @ColumnWidth(10)
    @Schema(description = "收退方向：收费 / 退费（导出时用中文，不用 CHARGE/REFUND 英文编码）")
    private String tradeDirection;

    @ExcelProperty("来源")
    @ColumnWidth(14)
    @Schema(description = "来源（字典翻译，如 门诊医生站/自助机）")
    private String sourceDisplay;

    @ExcelProperty("医保等级")
    @ColumnWidth(12)
    @Schema(description = "医保等级展示")
    private String insuranceLevelDisplay;

    @ExcelProperty("医保类型")
    @ColumnWidth(12)
    @Schema(description = "医保类型展示")
    private String insuranceTypeDisplay;

    // ============================================
    // 💰 Excel 金额列（退费场景请整体乘以 -1，最后一行为合计行）
    // ============================================
    @ExcelProperty("原价")
    @ColumnWidth(14)
    @Schema(description = "原价")
    private BigDecimal baseAmount;

    @ExcelProperty("优惠金额")
    @ColumnWidth(14)
    @Schema(description = "优惠金额")
    private BigDecimal discountAmount;

    @ExcelProperty("应收金额")
    @ColumnWidth(14)
    @Schema(description = "应收金额（= baseAmount - discountAmount）")
    private BigDecimal receivableAmount;

    @ExcelProperty("折后金额")
    @ColumnWidth(14)
    @Schema(description = "折后金额")
    private BigDecimal discountedAmount;

    @ExcelProperty("行金额")
    @ColumnWidth(14)
    @Schema(description = "行金额（含退单负号）")
    private BigDecimal lineAmount;

    @ExcelProperty("实收金额")
    @ColumnWidth(14)
    @Schema(description = "实收金额（含退单负号）")
    private BigDecimal actualReceivedAmount;

    @ExcelProperty("医保金额")
    @ColumnWidth(14)
    @Schema(description = "医保金额（含退单负号）")
    private BigDecimal insuranceAmount;

    @ExcelProperty("自费金额")
    @ColumnWidth(14)
    @Schema(description = "自费金额（含退单负号）")
    private BigDecimal selfPayAmount;
}