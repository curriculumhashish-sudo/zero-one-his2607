package com.zeroone.star.project.dto.j8.mzfymx;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.math.BigDecimal;

/**
 * <p>
 * 描述：门诊费用明细-单行数据 DTO（方案 B 纯平铺：1 行 = 1 条收费项，就诊字段每行重复）
 *      对应前端 Table 的一行，以及 Excel 导出的一行（MinXiExportDTO 仅少了主键和序号）
 *      ⚠️ 所有 ID 字段类型为 String（雪花/UUID 字符串，如 "335734645861126144"），不是 Long！
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 阿伟学长
 * @version 1.0.0
 */
@Data
@Schema(description = "门诊费用明细-单行数据（纯平铺，1行=1条收费项）")
public class MinXiDTO {

    // ============================================
    // 🔑 内部追踪字段（列表不展示，前端/后端追溯用）—— 全部改为 String（雪花 ID 字符串）
    // ============================================
    @Schema(description = "收费明细主键（雪花 ID 字符串，如 335734645970178048）")
    private String chargeItemId;

    @Schema(description = "关联支付流水主键（雪花 ID 字符串）")
    private String paymentId;

    // ============================================
    // 🏥 就诊级展示字段（方案 B 纯平铺：每行重复 1 次，对应前端 Table 前 5 列）
    // ⚠️ 同一次就诊多条收费项 → 这 4 列完全相同（如小赵上午挂号+开药，两行都重复"小赵/ENxxxx/门诊内科/2026-07-15"）
    // ============================================
    @Schema(description = "序号（分页连续递增：(pageIndex-1)*pageSize + rowIndex+1）")
    private Integer sequenceNo;

    @Schema(description = "门诊号（就诊号，来源 adm_encounter.bus_no，如 EN2026-07-150001）")
    private String outpatientNo;

    @Schema(description = "患者姓名（来源 adm_patient.name，如 小赵）")
    private String patientName;

    @Schema(description = "挂号科室名称（来源 adm_organization.name，如 门诊内科）")
    private String registrationDepartmentName;

    @Schema(description = "就诊日期（格式 yyyy-MM-dd，来源 adm_encounter.start_time 挂号/就诊开始时间 截取日期）")
    private String encounterDate;

    @Schema(description = "费用大类 code（来源 adm_charge_item_definition.type_code，如 1012/1001/2005）")
    private String feeCategoryCode;

    @Schema(description = "费用大类展示文案（如 挂号/药品/耗材/医疗服务）")
    private String feeCategoryDisplay;

    @Schema(description = "细分类 code（目前 def 表无 sub_type_code，暂与 feeCategoryCode 相同）")
    private String subCategoryCode;

    @Schema(description = "细分类展示文案（目前 def 表无 sub_type_code，暂与 feeCategoryDisplay 相同）")
    private String subCategoryDisplay;

    @Schema(description = "项目名称（优先取 3 张产品定义表 name：wkf/med/dev；挂号类取 def.charge_name）")
    private String productName;

    @Schema(description = "院内项目编码（来源 wkf_activity_definition / med_medication_definition / adm_device_definition 的 bus_no）")
    private String productCode;

    @Schema(description = "规格（当前产品表无 specification 字段，暂返回 NULL，后续接字典表后补充）")
    private String specification;

    @Schema(description = "数量")
    private BigDecimal quantityValue;

    @Schema(description = "数量单位展示（如 次/盒/袋）")
    private String quantityUnitDisplay;

    @Schema(description = "单价（adm_charge_item.unit_price）")
    private BigDecimal unitPrice;

    @Schema(description = "开单医生主键（雪花 ID 字符串）")
    private String orderingDoctorId;

    @Schema(description = "开单医生姓名")
    private String orderingDoctorName;

    @Schema(description = "收费人主键（雪花 ID 字符串）")
    private String cashierId;

    @Schema(description = "收费人姓名")
    private String cashierName;

    @Schema(description = "结算时间（格式：yyyy-MM-dd HH:mm:ss）")
    private String billDate;

    @Schema(description = "收退方向：CHARGE / REFUND（英文编码，中文展示导出接口再转）")
    private String tradeDirection;

    @Schema(description = "折扣率展示（如 100% / 90% / 自费 0%）")
    private String discountRateDisplay;

    @Schema(description = "来源（adm_charge_item.generateSourceEnum 字典翻译，如 门诊医生站/自助机）")
    private String sourceDisplay;

    @Schema(description = "医保等级展示（来源 adm_encounter.ybClassText，如 职工医保/居民医保）")
    private String insuranceLevelDisplay;

    @Schema(description = "医保类型展示（来源 adm_charge_item_definition.yb_type 字典翻译，如 甲类/乙类/自费）")
    private String insuranceTypeDisplay;

    // ============================================
    // 💰 金额列（退费行直接从数据库取负数，合计时自然抵消）
    // ============================================
    @Schema(description = "原价")
    private BigDecimal baseAmount;

    @Schema(description = "优惠金额")
    private BigDecimal discountAmount;

    @Schema(description = "应收金额（= baseAmount - discountAmount）")
    private BigDecimal receivableAmount;

    @Schema(description = "折后金额（原价 × 折扣率）")
    private BigDecimal discountedAmount;

    @Schema(description = "行金额（含退单负号）")
    private BigDecimal lineAmount;

    @Schema(description = "实收金额（含退单负号，聚合 fin_payment_rec_detail.charge_amount - return_amount）")
    private BigDecimal actualReceivedAmount;

    @Schema(description = "医保金额（含退单负号）")
    private BigDecimal insuranceAmount;

    @Schema(description = "自费金额（含退单负号，= lineAmount - insuranceAmount）")
    private BigDecimal selfPayAmount;
}