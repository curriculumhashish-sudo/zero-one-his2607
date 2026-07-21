package com.zeroone.star.j7.dispense.controller;

import com.zeroone.star.project.dto.j7.dispense.DispenseQuery;
import com.zeroone.star.project.j7.dispense.OutpatientDispenseApis;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.vo.j7.dispense.DispenseGroupVO;
import com.zeroone.star.project.vo.j7.dispense.DrugItemVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.servlet.http.HttpServletResponse;
import org.springframework.web.bind.annotation.RestController;

import java.io.IOException;
import java.math.BigDecimal;
import java.util.Arrays;
import java.util.List;

@RestController
@Tag(name = "门诊发退药记录", description = "门诊药房发药/退药记录查询与导出")
public class OutpatientDispenseController implements OutpatientDispenseApis {

    @Override
    @Operation(summary = "分页查询发退药记录", description = "按处方分组展示门诊发药/退药记录")
    public JsonVO<PageDTO<DispenseGroupVO>> queryPage(
            @Parameter(description = "查询参数", required = true) DispenseQuery query) {
        // 返回示例数据用于生成 Knife4j 文档
        PageDTO<DispenseGroupVO> pageResult = new PageDTO<>();
        pageResult.setPageIndex(1L);
        pageResult.setPageSize(20L);
        pageResult.setTotal(12L);
        pageResult.setPages(1L);

        DispenseGroupVO vo = new DispenseGroupVO();
        vo.setPrescriptionNo("CP2026-07-010003");
        vo.setRecordTypeName("发药");
        vo.setPatientName("马化腾");
        vo.setClinicNo("EN2026-07-010001");
        vo.setDeptName("门诊内科");
        vo.setDispenseTime("2026-07-01 19:25:06");
        vo.setPharmacyName("门诊中药房");
        vo.setDispenserName("admin");

        DrugItemVO drug1 = new DrugItemVO();
        drug1.setDrugName("白芍");
        drug1.setSpec("10g/包");
        drug1.setQuantity(BigDecimal.valueOf(3));
        drug1.setUnit("包");

        DrugItemVO drug2 = new DrugItemVO();
        drug2.setDrugName("当归");
        drug2.setSpec("10g/包");
        drug2.setQuantity(BigDecimal.valueOf(2));
        drug2.setUnit("包");

        DrugItemVO drug3 = new DrugItemVO();
        drug3.setDrugName("黄芪");
        drug3.setSpec("10g/包");
        drug3.setQuantity(BigDecimal.valueOf(3));
        drug3.setUnit("包");

        vo.setDrugItems(Arrays.asList(drug1, drug2, drug3));
        pageResult.setRows(List.of(vo));

        return JsonVO.success(pageResult);
    }

    @Override
    @Operation(summary = "导出发退药记录", description = "将查询结果以平铺格式导出为 Excel")
    public void export(
            @Parameter(description = "查询参数") DispenseQuery query,
            HttpServletResponse response) throws IOException {
        // TODO: 后续实现导出逻辑
        response.setContentType("application/vnd.openxmlformats-officedocument.spreadsheetml.sheet");
        response.setCharacterEncoding("UTF-8");
        response.setHeader("Content-Disposition", "attachment;filename=dispense_export.xlsx");
        response.getWriter().write("导出功能开发中...");
    }
}