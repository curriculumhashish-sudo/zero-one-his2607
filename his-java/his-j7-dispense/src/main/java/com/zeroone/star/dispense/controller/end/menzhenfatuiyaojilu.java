package com.zeroone.star.dispense.controller.end;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.j7.DispenseApis;
import com.zeroone.star.project.j7.dispense.OutpatientDispenseApis;
import com.zeroone.star.project.query.j7.dispense.DispenseQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j7.dispense.DispenseGroupVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.servlet.http.HttpServletResponse;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;

import java.io.IOException;


@RestController
@Tag(name = "门诊发退药记录", description = "门诊发退药记录基础流程")
public class menzhenfatuiyaojilu implements OutpatientDispenseApis {
     @Resource
    private IOutpatientDispenseRecordService service;

    @Resource
    private ExcelComponent excelComponent;

    @Override
    @GetMapping("/page")
    @Operation(summary = "分页查询发退药记录", description = "按处方分组展示门诊发药/退药记录")
    public JsonVO<PageDTO<DispenseGroupVO>> queryPage(
            @Parameter(description = "查询参数", required = true) DispenseQuery query) {
        return JsonVO.success(service.queryPage(query));
    }

    @Override
    @GetMapping("/export")
    @Operation(summary = "导出发退药记录", description = "将查询结果以平铺格式导出为 Excel")
    public void export(
            @Parameter(description = "查询参数") DispenseQuery query,
            HttpServletResponse response) throws IOException {
        List<OutpatientDispenseRecordDTO> records = service.listForExport(query);
        String filename = URLEncoder.encode("门诊发退药记录-" + LocalDate.now() + ".xlsx", StandardCharsets.UTF_8)
                .replace("+", "%20");
        response.setContentType("application/vnd.openxmlformats-officedocument.spreadsheetml.sheet");
        response.setCharacterEncoding(StandardCharsets.UTF_8.name());
        response.setHeader("Content-Disposition", "attachment; filename*=UTF-8''" + filename);
        excelComponent.export("门诊发退药记录", response.getOutputStream(), OutpatientDispenseRecordDTO.class, records);
    }
}
