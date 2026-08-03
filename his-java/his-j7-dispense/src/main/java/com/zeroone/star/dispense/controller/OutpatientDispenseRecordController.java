package com.zeroone.star.dispense.controller;

import com.zeroone.star.dispense.service.IOutpatientDispenseRecordService;
import com.zeroone.star.project.components.excel.ExcelComponent;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j7.dispense.OutpatientDispenseRecordDTO;
import com.zeroone.star.project.j7.dispense.OutpatientDispenseApis;
import com.zeroone.star.project.query.j7.dispense.DispenseQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j7.dispense.DispenseGroupVO;
import jakarta.annotation.Resource;
import jakarta.servlet.ServletOutputStream;
import jakarta.servlet.http.HttpServletResponse;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.net.URLEncoder;
import java.nio.charset.StandardCharsets;
import java.time.LocalDate;
import java.util.List;

/**
 * <p>
 * 描述：门诊发退药记录服务实现
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 再睡一夏
 * @version 1.0.0
 */

@RestController
@RequestMapping("/outpatient/dispense")
public class OutpatientDispenseRecordController implements OutpatientDispenseApis {

    @Resource
    private IOutpatientDispenseRecordService service;

    @Resource
    private ExcelComponent excelComponent;

    @Override
    @GetMapping("/page")
    public JsonVO<PageDTO<DispenseGroupVO>> queryPage(DispenseQuery query) {
        return JsonVO.success(service.queryPage(query));
    }

    @Override
    @GetMapping("/export")
    public void export(DispenseQuery query,
            HttpServletResponse response) throws IOException {
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
        ServletOutputStream sos = response.getOutputStream();
        sos.write(bytes);
        sos.flush();
    }
}