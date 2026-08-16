package com.zeroone.star.dispense.controller.end;

import com.zeroone.star.dispense.service.IInpatientDispenseRecordService;
import com.zeroone.star.project.components.excel.ExcelComponent;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j7.dispense.InpatientDispenseRecordDTO;
import com.zeroone.star.project.j7.dispense.InpatientDispenseRecordApis;
import com.zeroone.star.project.query.j7.dispense.InpatientDispenseRecordQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j7.dispense.InpatientDispenseRecordPageJsonVO;
import com.zeroone.star.project.vo.j7.dispense.InpatientDispenseRecordVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.media.Content;
import io.swagger.v3.oas.annotations.media.Schema;
import io.swagger.v3.oas.annotations.responses.ApiResponse;
import io.swagger.v3.oas.annotations.responses.ApiResponses;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import jakarta.servlet.ServletOutputStream;
import jakarta.servlet.http.HttpServletResponse;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.RestController;

import java.io.IOException;
import java.net.URLEncoder;
import java.nio.charset.StandardCharsets;
import java.time.LocalDate;
import java.util.List;

@RestController
@Validated
@Tag(name = "住院发退药记录", description = "住院发退药记录基础流程")
public class zhuyuanfatuiyaojilu implements InpatientDispenseRecordApis {

    @Resource
    private IInpatientDispenseRecordService service;

    @Resource
    private ExcelComponent excelComponent;
 
    @Override
    @ApiResponses({
        @ApiResponse(responseCode = "200", description = "操作成功",
            content = @Content(schema = @Schema(implementation = InpatientDispenseRecordPageJsonVO.class,
                example = "{\"code\":10000,\"message\":\"操作成功\",\"data\":{\"pageIndex\":1,\"pageSize\":10,\"total\":4,\"pages\":1,\"rows\":[{\"id\":\"990000000000000004\",\"dispenseTypeName\":\"退药\",\"dispenseStatusName\":\"PENDING\",\"prescriptionNo\":\"MR-TEST-REFUND-004\",\"patientName\":\"小赵\",\"inpatientNo\":\"ZY2026-06-300002\",\"wardName\":\"内科疗区\",\"bedNo\":\"1号床\",\"drugName\":\"氯化钠注射液\",\"spec\":\"250ml:2.25g\",\"dispenseQuantity\":2.00,\"unitCode\":\"8\",\"lotNumber\":\"LOT-TEST-004\",\"traceNo\":null,\"pharmacistName\":\"admin\",\"dispenseTime\":null},{\"id\":\"990000000000000003\",\"dispenseTypeName\":\"退药\",\"dispenseStatusName\":\"已退药\",\"prescriptionNo\":\"MR-TEST-REFUND-003\",\"patientName\":\"周女士\",\"inpatientNo\":\"ZY2026-06-300001\",\"wardName\":\"内科疗区\",\"bedNo\":\"1号床\",\"drugName\":\"注射用头孢呋辛钠\",\"spec\":\"0.75g\",\"dispenseQuantity\":1.00,\"unitCode\":\"8\",\"lotNumber\":\"LOT-TEST-003\",\"traceNo\":null,\"pharmacistName\":\"admin\",\"dispenseTime\":null}]}}")))
    })
    public JsonVO<PageDTO<InpatientDispenseRecordVO>> queryPage(InpatientDispenseRecordQuery query) {
        return JsonVO.success(service.queryPage(query));
    }

    @Override
    @ApiResponses({
        @ApiResponse(responseCode = "200", description = "导出成功，返回Excel文件流")
    })
    public void export(InpatientDispenseRecordQuery query, HttpServletResponse response) throws IOException {
        try {
            List<InpatientDispenseRecordDTO> records = service.listForExport(query);
            String filename = URLEncoder.encode("住院发退药记录-" + LocalDate.now() + ".xlsx", StandardCharsets.UTF_8)
                    .replace("+", "%20");
            response.setContentType("application/vnd.openxmlformats-officedocument.spreadsheetml.sheet");
            response.setCharacterEncoding(StandardCharsets.UTF_8.name());
            response.setHeader("Content-Disposition", "attachment; filename*=UTF-8''" + filename);
            excelComponent.export("住院发退药记录", response.getOutputStream(), InpatientDispenseRecordDTO.class, records);
        } catch (IllegalArgumentException e) {
            response.setContentType("application/json;charset=UTF-8");
            response.setStatus(HttpServletResponse.SC_BAD_REQUEST);
            response.getWriter().write("{\"code\":9999,\"message\":\"" + e.getMessage() + "\",\"data\":null}");
        }
    }
}
