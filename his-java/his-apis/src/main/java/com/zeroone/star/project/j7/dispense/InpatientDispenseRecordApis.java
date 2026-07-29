package com.zeroone.star.project.j7.dispense;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.query.j7.dispense.InpatientDispenseRecordQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j7.dispense.InpatientDispenseRecordVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.servlet.http.HttpServletResponse;
import jakarta.validation.Valid;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.ModelAttribute;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;

import java.io.IOException;

@Tag(name = "住院发退药记录")
@RequestMapping("/pharmacy/dispense-record/inpatient")
public interface InpatientDispenseRecordApis {

    @Operation(summary = "分页查询住院发退药记录")
    @PostMapping("/page")
    JsonVO<PageDTO<InpatientDispenseRecordVO>> queryPage(
            @Parameter(description = "分页及筛选条件", required = true)
            @Valid @RequestBody InpatientDispenseRecordQuery query
    );

    @Operation(summary = "按当前筛选条件导出住院发退药记录")
    @GetMapping("/export")
    void export(
            @Parameter(description = "筛选条件，不使用分页参数")
            @Valid @ModelAttribute InpatientDispenseRecordQuery query,
            HttpServletResponse response
    ) throws IOException;
}
