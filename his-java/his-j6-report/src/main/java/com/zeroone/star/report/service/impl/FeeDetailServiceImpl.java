package com.zeroone.star.report.service.impl;

import com.zeroone.star.project.query.j6.report.FeeDetailExportQuery;
import com.zeroone.star.project.query.j6.report.FeeDetailQuery;
import com.zeroone.star.project.components.excel.ExcelComponent;
import com.zeroone.star.project.vo.j6.report.FeeDetailVO;
import com.zeroone.star.project.vo.j6.report.FeeSummaryVo;
import com.zeroone.star.project.vo.j6.report.OutpatientFeeDetailsVo;
import com.zeroone.star.report.entity.FeeDetailExportDO;
import com.zeroone.star.report.entity.FeeDetailLineDO;
import com.zeroone.star.report.entity.FeeDetailSummaryDO;
import com.zeroone.star.report.entity.PageEncounterDO;
import com.zeroone.star.report.mapper.AdmEncounterMapper;
import com.zeroone.star.report.service.IFeeDetailService;
import jakarta.annotation.Resource;
import jakarta.servlet.http.HttpServletResponse;
import org.springframework.stereotype.Service;

import java.io.IOException;
import java.math.BigDecimal;
import java.net.URLEncoder;
import java.nio.charset.StandardCharsets;
import java.time.LocalDate;
import java.time.format.DateTimeFormatter;
import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

/**
 * 门诊费用明细 Service 实现
 * @author J6
 */
@Service
public class FeeDetailServiceImpl implements IFeeDetailService {

    private static final DateTimeFormatter DATE_FMT = DateTimeFormatter.ofPattern("yyyy-MM-dd");
    private static final DateTimeFormatter DATE_TIME_FMT = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss");

    @Resource
    private AdmEncounterMapper admEncounterMapper;

    @Resource
    private ExcelComponent excelComponent;

    @Override
    public OutpatientFeeDetailsVo pageFeeDetailQuery(FeeDetailQuery query) {

        // 参数校验
        if (!tradeDirectionValid(query.getTradeDirection())) {
            throw new RuntimeException("收退方向只能为ALL/CHARGE/REFUND");
        }

        // 1. 手动分页：先统计总数，再查当前页
        long pageIndex = query.getPageIndex() > 0 ? query.getPageIndex() : 1L;
        long pageSize = query.getPageSize() > 0 ? query.getPageSize() : 10L;
        long total = admEncounterMapper.selectEncounterCount(query);
        long pages = (total + pageSize - 1) / pageSize;
        long offset = (pageIndex - 1) * pageSize;

        List<PageEncounterDO> encounters = admEncounterMapper.selectEncounterByPage(query, offset, pageSize);

        // 2. 收集就诊ID
        List<String> encounterIds = encounters.stream()
                .map(PageEncounterDO::getEncounterId)
                .toList();

        // 3. 批量查询费用明细行
        List<FeeDetailLineDO> allLines = encounterIds.isEmpty()
                ? List.of()
                : admEncounterMapper.selectFeeDetailLineByEncounterIds(encounterIds, query);

        // 4. 按 encounterId 分组
        Map<String, List<FeeDetailLineDO>> grouped = new LinkedHashMap<>();
        for (FeeDetailLineDO line : allLines) {
            grouped.computeIfAbsent(line.getEncounterId(), k -> new ArrayList<>()).add(line);
        }

        // 5. 组装 FeeDetailVO（保持分页顺序）
        List<FeeDetailVO> rows = new ArrayList<>();
        for (PageEncounterDO encounter : encounters) {
            FeeDetailVO vo = new FeeDetailVO();
            vo.setEncounterId(encounter.getEncounterId());
            vo.setBusNo(encounter.getBusNo());
            vo.setPatientName(encounter.getPatientName());
            vo.setRegistrationDepartmentName(encounter.getRegistrationDepartmentName());
            vo.setEncounterDate(encounter.getEncounterDate() != null
                    ? encounter.getEncounterDate().format(DATE_FMT) : null);

            // 费用行
            List<FeeDetailLineDO> encLines = grouped.getOrDefault(encounter.getEncounterId(), List.of());
            List<FeeDetailVO.FeeDetail> detailLines = new ArrayList<>();
            BigDecimal[] sub = initSubTotal();
            for (FeeDetailLineDO l : encLines) {
                detailLines.add(toFeeDetail(l));
                sub[0] = sub[0].add(nvl(l.getBaseAmount()));                     // baseAmountTotal
                sub[1] = sub[1].add(nvl(l.getDiscountedAmount()));              // discountAmountTotal (折扣金额)
                sub[2] = sub[2].add(nvl(l.getDiscountedAmount()));              // receivableAmountTotal (应收≈折扣金额)
                sub[3] = sub[3].add(nvl(l.getDiscountAmount()));                // discountedAmountTotal (折后净值)
                sub[4] = sub[4].add(nvl(l.getDiscountAmount()));                // actualReceivedAmountTotal (实收≈折后)
                sub[5] = sub[5].add(nvl(l.getInsuranceAmount()));               // insuranceAmountTotal
                sub[6] = sub[6].add(nvl(l.getDiscountedAmount()));              // selfPayAmountTotal (自费≈折扣金额)
            }
            vo.setLines(detailLines);

            // subTotal
            FeeSummaryVo subTotal = new FeeSummaryVo();
            subTotal.setBaseAmountTotal(sub[0]);
            subTotal.setDiscountAmountTotal(sub[1]);
            subTotal.setReceivableAmountTotal(sub[2]);
            subTotal.setDiscountedAmountTotal(sub[3]);
            subTotal.setActualReceivedAmountTotal(sub[4]);
            subTotal.setInsuranceAmountTotal(sub[5]);
            subTotal.setSelfPayAmountTotal(sub[6]);
            vo.setSubTotal(subTotal);

            rows.add(vo);
        }

        // 6. 构建分页结果
        OutpatientFeeDetailsVo result = new OutpatientFeeDetailsVo();
        result.setPageIndex(pageIndex);
        result.setPageSize(pageSize);
        result.setTotalCount(total);
        result.setContent(rows);

        // 7. 全局汇总
        FeeDetailSummaryDO summary = admEncounterMapper.selectFeeDetailSummary(query);
        FeeSummaryVo summaryVo = new FeeSummaryVo();
        if (summary != null) {
            summaryVo.setBaseAmountTotal(nvl(summary.getBaseAmountTotal()));
            summaryVo.setDiscountAmountTotal(nvl(summary.getDiscountAmountTotal()));
            summaryVo.setDiscountedAmountTotal(nvl(summary.getDiscountedAmountTotal()));
            summaryVo.setReceivableAmountTotal(nvl(summary.getReceivableAmountTotal()));
            summaryVo.setLineAmountTotal(nvl(summary.getDiscountedAmountTotal()));
            summaryVo.setActualReceivedAmountTotal(nvl(summary.getActualReceivedAmountTotal()));
            summaryVo.setInsuranceAmountTotal(nvl(summary.getInsuranceAmountTotal()));
            summaryVo.setSelfPayAmountTotal(nvl(summary.getSelfPayAmountTotal()));
        }
        result.setSummary(summaryVo);

        return result;
    }

    @Override
    public void exportFeeDetail(FeeDetailExportQuery query, HttpServletResponse response) {
        // 1. 全量查询（无分页）
        List<FeeDetailExportDO> rows = admEncounterMapper.selectAllExportLines(query);

        // 2. 设置响应头
        String filename = "门诊费用明细_" + LocalDate.now().format(DateTimeFormatter.ofPattern("yyyyMMdd")) + ".xlsx";
        response.setContentType("application/vnd.openxmlformats-officedocument.spreadsheetml.sheet");
        response.setCharacterEncoding(StandardCharsets.UTF_8.name());
        response.setHeader("Content-Disposition",
                "attachment; filename=" + URLEncoder.encode(filename, StandardCharsets.UTF_8).replace("+", "%20"));

        // 3. 写入 Excel 到 response 输出流
        try {
            excelComponent.export("门诊费用明细", response.getOutputStream(), FeeDetailExportDO.class, rows);
        } catch (IOException e) {
            throw new RuntimeException("导出 Excel 失败", e);
        }
    }

    // ---- helper ----

    private FeeDetailVO.FeeDetail toFeeDetail(FeeDetailLineDO l) {
        FeeDetailVO.FeeDetail d = new FeeDetailVO.FeeDetail();
        d.setChargeItemId(l.getChargeItemId());
        d.setFeeCategoryCode(l.getFeeCategoryCode());
        d.setFeeCategoryDisplay(l.getFeeCategoryDisplay());
        d.setSubCategoryCode(l.getSubCategoryCode());
        d.setSubCategoryDisplay(l.getSubCategoryDisplay());
        d.setProductName(l.getProductName());
        d.setProductCode(l.getProductCode());
        d.setSpecification(l.getSpecification());
        d.setQuantityValue(l.getQuantityValue());
        d.setQuantityUnitDisplay(l.getQuantityUnitDisplay());
        d.setOrderingDoctorId(l.getOrderingDoctorId());
        d.setOrderingDoctorName(l.getOrderingDoctorName());
        d.setCashierId(l.getCashierId());
        d.setCashierName(l.getCashierName());
        d.setBillDate(l.getBillDate() != null ? l.getBillDate().format(DATE_TIME_FMT) : null);
        d.setTradeDirection(l.getTradeDirection());
        d.setInsuranceLevelDisplay(l.getInsuranceLevelDisplay());
        d.setInsuranceTypeDisplay(l.getInsuranceTypeDisplay());
        d.setBaseAmount(nvl(l.getBaseAmount()));
        d.setUnitAmount(nvl(l.getUnitAmount()));
        d.setUnitPrice(nvl(l.getUnitAmount()));                  // 前端字段名
        d.setLineAmount(nvl(l.getDiscountedAmount()));          // 行金额
        d.setDiscountAmount(nvl(l.getDiscountedAmount()));      // 折扣金额 ← SQL discountedAmount(折扣)
        d.setDiscountedAmount(nvl(l.getDiscountAmount()));      // 折后金额 ← SQL discountAmount(base-discount)
        d.setReceivableAmount(nvl(l.getDiscountedAmount()));    // 应收金额 ≈ 折扣金额
        d.setActualReceivedAmount(nvl(l.getDiscountAmount()));  // 实收金额 ≈ 折后净值
        d.setInsuranceAmount(nvl(l.getInsuranceAmount()));
        d.setSelfPayAmount(nvl(l.getDiscountedAmount()));       // 自费金额 ≈ 折扣金额
        return d;
    }

    private BigDecimal nvl(BigDecimal v) {
        return v != null ? v : BigDecimal.ZERO;
    }

    private BigDecimal[] initSubTotal() {
        return new BigDecimal[]{
                BigDecimal.ZERO, BigDecimal.ZERO, BigDecimal.ZERO,
                BigDecimal.ZERO, BigDecimal.ZERO, BigDecimal.ZERO, BigDecimal.ZERO
        };
    }

    // 收退方向枚举校验
    private boolean tradeDirectionValid(String tradeDirection) {
        if (tradeDirection != null && !tradeDirection.isEmpty()) {
            return tradeDirection.equals("ALL") || tradeDirection.equals("CHARGE") || tradeDirection.equals("REFUND");
        }

        return true;
    }

    // 日期范围校验
//    private boolean dateTimeValid(String dateFrom, String dateTo) {
//        if ()
//
//        return dateFrom.compareTo(dateTo) <= 0;
//    }
}
