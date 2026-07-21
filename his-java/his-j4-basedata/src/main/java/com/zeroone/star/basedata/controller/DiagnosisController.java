package com.zeroone.star.basedata.controller;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.j4.basedata.DiagnosisApis;
import com.zeroone.star.project.query.j4.diagnosis.DiagnosisPageQuery;
import com.zeroone.star.project.query.j4.diagnosis.DiagnosisSelectPageQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j4.diagnosis.DiagnosisDetailVO;
import com.zeroone.star.project.vo.j4.diagnosis.DiagnosisListVO;
import com.zeroone.star.project.vo.j4.diagnosis.DiagnosisOptionVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.validation.Valid;
import org.springframework.web.bind.annotation.*;

/**
 * <p>
 * 描述：诊断管理控制器
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@RestController
@RequestMapping("/diagnosis")
@Tag(name = "basedata-diagnosis", description = "诊断管理接口")
public class DiagnosisController implements DiagnosisApis {

    @Override
    @Operation(summary = "获取选择诊断列表（条件+分页）")
    @GetMapping("/query-select-list")
    public JsonVO<PageDTO<DiagnosisOptionVO>> queryDiagnosisSelectList(
            @Valid DiagnosisSelectPageQuery query) {
        return JsonVO.success(null);
    }

    @Override
    @Operation(summary = "获取诊断列表（条件+分页）")
    @GetMapping("/query-list")
    public JsonVO<PageDTO<DiagnosisListVO>> queryDiagnosisList(@Valid DiagnosisPageQuery query) {
        return JsonVO.success(null);
    }

    @Override
    @Operation(summary = "获取诊断详情")
    @GetMapping("/query-detail")
    public JsonVO<DiagnosisDetailVO> queryDiagnosisDetail(@RequestParam String id) {
        return JsonVO.success(null);
    }
}
