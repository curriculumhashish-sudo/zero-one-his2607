package com.zeroone.star.outpatient.mapper;

import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.project.dto.j8.mzfymx.MinXiDTO;
import com.zeroone.star.project.query.j8.mzfymx.MinXiQuery;
import com.zeroone.star.project.vo.j8.mzfymx.MinXiPageVO;
import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;

import java.util.List;

/**
 * <p>
 * 描述：门诊费用明细-自定义联查 Mapper（核心数据访问层）
 *      ⚠️ 2026-07-26 数据库 zo_his 全部字段实锤版，以下所有注释 100% 与 MinXiMapper.xml 一致
 *      ⚠️ 其他 13 张单表 Mapper 已精简删除，所有联表查询 SQL 统一写在 MinXiMapper.xml 里
 *      🔴 核心修正历史（踩坑记录，避免以后回退）：
 *      1) enc.occurrence_time 不存在 → 用 enc.start_time
 *      2) aci.name_text 不存在 → 从 wkf/med/dev 产品表 name + def.charge_name 取
 *      3) med/dev/wkf 都没有 specification 字段 → 规格暂返回 NULL
 *      4) def 表 JOIN 条件：aci.def_detail_id 全 NULL → 用 def.id = aci.definition_id（真实有值）
 *      5) def 表没有 sub_type_code / bus_no 字段 → 细分类暂复用 type_code，院内编码从产品表取
 *      6) rec.kind_enum 只有 OUTPATIENT_CLINIC / HOSPITAL_DEPOSIT，没有 REFUND
 *         → 退费判断一律用 aci.refund_id IS NOT NULL
 *      7) aci.system_discount_price 全 NULL → 折后金额用 (base_amount - discount_amount)
 *      8) aci.total_price = aci.base_amount = 数量×单价 =【原价】不是折后
 *         → lineAmount / selfPayAmount 一律用 (base_amount - discount_amount) 不能用 total_price！
 *      9) rec.enterer_id（收银员）跨两张表：ID=1 在 whale_users=admin；ID=308791351298756608 在 adm_practitioner=admin
 *         → 必须 LEFT JOIN 两张表 + COALESCE 合并
 *     10) 费用分类优先 product_table 判断（100%准）：med_medication_definition=药品，adm_device_definition=耗材，wkf_activity_definition=医疗服务
 *         不要用 def.type_code 前缀（药品的 type_code 常是 1001/1003，不是 200x！会把耗材误判成药品）
 *     11) doc = aci.enterer_id（收费项录入人=开单医生），不是 performer_id
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author Zzen
 * @version 1.0.0
 */
@Mapper
public interface MinXiMapper {

    /**
     * 1. 全量合计（8 个金额 SUM，不带 LIMIT，对应原型图顶部红色数字）
     *   🔴 8 个 *Sum = MinXiPageVO 中的属性名（resultType 自动映射）：
     *      1) baseAmountSum          = SUM(±base_amount)
     *      2) discountAmountSum      = SUM(±discount_amount)
     *      3) receivableAmountSum    = SUM(±(base_amount - discount_amount))  应收 = 原价 - 优惠
     *      4) discountedAmountSum    = SUM(±(base_amount - discount_amount))  折后 = 原价 - 优惠
     *      5) lineAmountSum          = SUM(±(base_amount - discount_amount))  行金额 = 折后 = 原价 - 优惠（❗不能用 total_price = 原价）
     *      6) actualReceivedAmountSum = SUM(±(det.charge_amount - det.return_amount)) 实收 = 对账实收 - 对账实退
     *      7) insuranceAmountSum     = 实收里 pay_enum='YB' 的部分（医保）
     *      8) selfPayAmountSum       = lineAmountSum - insuranceAmountSum（自费 = 行金额折后 - 医保）
     *   🔴 ± 号规则：aci.refund_id IS NOT NULL（退费行）统一取负数，合计时自然抵消
     *
     *   🔴 JOIN 结构（与 selectRows 100% 相同，复用 minXiJoinFragment + minXiWhereFragment）：
     *   FROM adm_charge_item aci
     *     LEFT JOIN adm_encounter enc                ON enc.id = aci.encounter_id
     *     LEFT JOIN adm_patient pat                  ON pat.id = aci.patient_id
     *     LEFT JOIN adm_organization org             ON org.id = enc.organization_id
     *     LEFT JOIN adm_charge_item_definition def   ON def.id = aci.definition_id
     *     LEFT JOIN wkf_activity_definition wkf      ON aci.product_table = 'wkf_activity_definition' AND wkf.id = aci.product_id
     *     LEFT JOIN med_medication_definition med    ON aci.product_table = 'med_medication_definition' AND med.id = aci.product_id
     *     LEFT JOIN adm_device_definition dev        ON aci.product_table = 'adm_device_definition' AND dev.id = aci.product_id
     *     LEFT JOIN fin_payment_reconciliation rec   ON rec.charge_item_ids LIKE CONCAT('%', aci.id, '%') AND rec.kind_enum = 'OUTPATIENT_CLINIC'
     *     LEFT JOIN fin_payment_rec_detail det       ON det.reconciliation_id = rec.id
     *     LEFT JOIN whale_users cashier              ON cashier.id = rec.enterer_id   ← 系统用户类收银员
     *     LEFT JOIN adm_practitioner prac_cashier    ON prac_cashier.id = rec.enterer_id ← 医护类收银员（❗不能漏）
     *     LEFT JOIN adm_practitioner doc             ON doc.id = aci.enterer_id
     *   <where>
     *     aci.is_deleted = 0 AND rec.kind_enum = 'OUTPATIENT_CLINIC'
     *     + 8 个 <if> 条件（dateFrom/dateTo/tradeDirection/deptId/feeType/feeTypeDetail/patientKeyword/projectName/internalCode）
     *   </where>
     *
     * @param query 查询条件（MinXiQuery，所有字段都对应 XML 中 <if test="q.xxx">）
     * @return MinXiPageVO（只有 8 个 *Sum 字段有值，rows / totalRowCount 空，由 Service 层组合）
     */
    MinXiPageVO selectTotals(@Param("q") MinXiQuery query);

    /**
     * 2. 分页明细行查询（JOIN 结构与 selectTotals 100% 相同，按结算时间 + aci.id 倒序）
     *    🔴 SELECT 列别名 = MinXiDTO 属性名（驼峰一致，resultType 自动映射）：
     *       内部 ID：chargeItemId / paymentId
     *       就诊展示：outpatientNo(enc.bus_no) / patientName(pat.name) / deptName(org.name) / encounterDate(DATE_FORMAT enc.start_time)
     *       收费项展示：feeCategoryCode / feeCategoryDisplay / subCategoryCode / subCategoryDisplay
     *                   productName(COALESCE wkf/med/dev name + def.charge_name 兜底) / productCode(3张产品表 bus_no)
     *                   specification(NULL，3张产品表都没这个字段) / quantityValue / quantityUnitDisplay / unitPrice
     *       人员类：orderingDoctorId(doc.id=aci.enterer_id) / orderingDoctorName(doc.name)
     *               cashierId(COALESCE whale_users.id, prac_cashier.id) / cashierName(COALESCE 两张表name)
     *       时间类：billDate(DATE_FORMAT rec.bill_date)
     *       枚举类：tradeDirection(CASE refund_id) / sourceDisplay(aci.generate_source_enum) /
     *               discountRateDisplay(CASE 计算 (base-discount)/base*100 + '%')
     *               insuranceLevelDisplay(enc.yb_class_text) / insuranceTypeDisplay(def.yb_type)
     *       8 个金额：baseAmount / discountAmount / receivableAmount / discountedAmount / lineAmount
     *                actualReceivedAmount / insuranceAmount / selfPayAmount
     *                退费行一律取负数（与 selectTotals 算法完全一致）
     *
     * @param page  MyBatis-Plus 分页参数（new Page(pageIndex, pageSize)）
     * @param query 查询条件，与 selectTotals 100% 共用同一套 WHERE 片段
     * @return 当前页行数据 List<MinXiDTO>，别名已与 DTO 属性一致，MyBatis 自动 set
     */
    List<MinXiDTO> selectRows(@Param("page") Page<MinXiDTO> page, @Param("q") MinXiQuery query);

    /**
     * 3. 导出用 - 查询全量明细（不带分页 LIMIT，复用 selectRows 的 SELECT 列 + JOIN 片段 + WHERE 片段）
     *    ⚠️ 与 selectRows 的区别：
     *      - 入参少了 Page 参数（不会自动加 LIMIT）
     *      - 入参类型改为 MinXiExportQuery（属性与 MinXiQuery 9个筛选字段完全一致）
     *      - 排序与 selectRows 相同（bill_date DESC, aci.id DESC）
     *    ⚠️ 返回结果与 selectRows 完全相同：List<MinXiDTO>（上层用 MapStruct 转为 MinXiExportDTO）
     *
     * @param query 导出查询条件（9个筛选字段，不含 pageIndex/pageSize）
     * @return 满足条件的全量明细行（不带 LIMIT）
     */
    List<MinXiDTO> selectAllRows(@Param("q") com.zeroone.star.project.query.j8.mzfymx.MinXiExportQuery query);

}