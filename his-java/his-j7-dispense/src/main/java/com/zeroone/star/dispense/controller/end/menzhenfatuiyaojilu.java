package com.zeroone.star.dispense.controller.end;

import com.zeroone.star.dispense.service.IOutpatientDispenseRecordService;
import com.zeroone.star.project.components.excel.ExcelComponent;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j7.dispense.OutpatientDispenseRecordDTO;
import com.zeroone.star.project.j7.dispense.OutpatientDispenseApis;
import com.zeroone.star.project.query.j7.dispense.DispenseQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j7.dispense.DispenseGroupVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.media.Content;
import io.swagger.v3.oas.annotations.media.Schema;
import io.swagger.v3.oas.annotations.responses.ApiResponse;
import io.swagger.v3.oas.annotations.responses.ApiResponses;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import jakarta.servlet.ServletOutputStream;
import jakarta.servlet.http.HttpServletResponse;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.net.URLEncoder;
import java.nio.charset.StandardCharsets;
import java.time.LocalDate;
import java.util.List;

@RestController
@RequestMapping("/outpatient/dispense")
@Validated
@Tag(name = "门诊发退药记录", description = "门诊药房发药/退药记录查询与导出")
public class menzhenfatuiyaojilu implements OutpatientDispenseApis {

    @Resource
    private IOutpatientDispenseRecordService service;

    @Resource
    private ExcelComponent excelComponent;

    @Override
    @GetMapping("/page")
    @Operation(summary = "分页查询发退药记录", description = "按处方分组展示门诊发药/退药记录")
    @ApiResponses({
        @ApiResponse(responseCode = "200", description = "操作成功",
            content = @Content(schema = @Schema(implementation = JsonVO.class)))
    })
    public JsonVO<PageDTO<DispenseGroupVO>> queryPage(
            @Parameter(description = "查询参数", required = true) DispenseQuery query) {
        return JsonVO.success(service.queryPage(query));
    }

    @Override
    @GetMapping("/export")
    @Operation(summary = "导出发退药记录", description = "将查询结果以平铺格式导出为 Excel")
    @ApiResponses({
        @ApiResponse(responseCode = "200", description = "导出成功，返回Excel文件流")
    })
    public void export(
            @Parameter(description = "查询参数") DispenseQuery query,
            HttpServletResponse response) throws IOException {
        try {
            List<OutpatientDispenseRecordDTO> records = service.listForExport(query);
            if (records == null || records.isEmpty()) {
                response.setContentType("application/json;charset=UTF-8");
                response.getWriter().write("{\"code\":0,\"msg\":\"没有可导出的数据\"}");
                return;
            }
            ByteArrayOutputStream baos = new ByteArrayOutputStream();
            excelComponent.export("门诊发退药记录", baos, OutpatientDispenseRecordDTO.class, records);
            byte[] bytes = baos.toByteArray();
            String filename = URLEncoder.encode("门诊发退药记录-" + LocalDate.now() + ".xlsx", StandardCharsets.UTF_8)
                    .replace("+", "%20");
            response.setContentType("application/vnd.openxmlformats-officedocument.spreadsheetml.sheet");
            response.setHeader("Content-Disposition", "attachment; filename=" + filename);
            response.setContentLength(bytes.length);
            ServletOutputStream outputStream = response.getOutputStream();
            outputStream.write(bytes);
            outputStream.flush();
        } catch (IllegalArgumentException e) {
            response.setContentType("application/json;charset=UTF-8");
            response.setStatus(HttpServletResponse.SC_BAD_REQUEST);
            response.getWriter().write("{\"code\":9999,\"message\":\"" + e.getMessage() + "\",\"data\":null}");
        }
    }
}
