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
    @Override
    @GetMapping("/page")
    @Operation(summary = "分页查询发退药记录", description = "按处方分组展示门诊发药/退药记录")
    public JsonVO<PageDTO<DispenseGroupVO>> queryPage(
            @Parameter(description = "查询参数", required = true) DispenseQuery query) {
        return JsonVO.success(null);
    }

    @GetMapping("/export")
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
