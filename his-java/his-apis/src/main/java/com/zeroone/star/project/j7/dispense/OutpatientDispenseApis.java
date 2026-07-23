package com.zeroone.star.project.j7.dispense;

import com.zeroone.star.project.query.j7.dispense.DispenseQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.vo.j7.dispense.DispenseGroupVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.servlet.http.HttpServletResponse;

import java.io.IOException;

@Tag(name = "门诊发退药记录")
public interface OutpatientDispenseApis {

    @Operation(summary = "分页查询发退药记录（按处方分组）")
    JsonVO<PageDTO<DispenseGroupVO>> queryPage(
            @Parameter(description = "查询参数", required = true) DispenseQuery query
    );

    @Operation(summary = "导出发退药记录（平铺格式）")
    void export(
            @Parameter(description = "查询参数") DispenseQuery query,
            HttpServletResponse response
    ) throws IOException;
}
