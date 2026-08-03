package com.zeroone.star.dispense.controller.end;

import com.zeroone.star.dispense.service.IInpatientDispenseRecordService;
import com.zeroone.star.project.components.excel.ExcelComponent;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j7.dispense.InpatientDispenseRecordDTO;
import com.zeroone.star.project.j7.dispense.InpatientDispenseRecordApis;
import com.zeroone.star.project.query.j7.dispense.InpatientDispenseRecordQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j7.dispense.InpatientDispenseRecordVO;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
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
    public JsonVO<PageDTO<InpatientDispenseRecordVO>> queryPage(InpatientDispenseRecordQuery query) {
        return JsonVO.success(service.queryPage(query));
    }

    @Override
    public void export(InpatientDispenseRecordQuery query, HttpServletResponse response) throws IOException {
        List<InpatientDispenseRecordDTO> records = service.listForExport(query);
        String filename = URLEncoder.encode("住院发退药记录-" + LocalDate.now() + ".xlsx", StandardCharsets.UTF_8)
                .replace("+", "%20");
        response.setContentType("application/vnd.openxmlformats-officedocument.spreadsheetml.sheet");
        response.setCharacterEncoding(StandardCharsets.UTF_8.name());
        response.setHeader("Content-Disposition", "attachment; filename*=UTF-8''" + filename);
        excelComponent.export("住院发退药记录", response.getOutputStream(), InpatientDispenseRecordDTO.class, records);
    }
}
