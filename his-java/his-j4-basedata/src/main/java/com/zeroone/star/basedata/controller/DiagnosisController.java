package com.zeroone.star.basedata.controller;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j4.diagnosis.DiagnosisDefinitionDTO;
import com.zeroone.star.project.j4.basedata.DiagnosisApis;
import com.zeroone.star.project.query.j4.diagnosis.DiagnosisPageQuery;
import com.zeroone.star.project.query.j4.diagnosis.DiagnosisSelectPageQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j4.diagnosis.DiagnosisDefinitionDetailVO;
import com.zeroone.star.project.vo.j4.diagnosis.DiagnosisDetailVO;
import com.zeroone.star.project.vo.j4.diagnosis.DiagnosisListVO;
import com.zeroone.star.project.vo.j4.diagnosis.DiagnosisOptionVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.validation.Valid;
import org.springframework.web.bind.annotation.DeleteMapping;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.PutMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

import java.util.List;

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
@Tag(name = "diagnosis", description = "诊断管理")
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
    @GetMapping("/get-detail/{id}")
    public JsonVO<DiagnosisDetailVO> queryDiagnosisDetail(@PathVariable String id) {
        return JsonVO.success(null);
    }

    @Override
    @Operation(summary = "新增诊断")
    @PostMapping("/add-definition")
    public JsonVO<String> addDefinition(@RequestBody DiagnosisDefinitionDTO dto) {
        return null;
    }

    @Override
    @Operation(summary = "修改诊断")
    @PutMapping("/modify-definition")
    public JsonVO<String> modifyDefinition(@RequestBody DiagnosisDefinitionDTO dto) {
        return null;
    }

    @Override
    @Operation(summary = "启用或停用诊断（支持批量）")
    @PutMapping("/modify-batch-status")
    public JsonVO<String> modifyBatchStatus(
            @RequestBody List<String> ids,
            @RequestParam String status) {
        return null;
    }

    @Override
    @Operation(summary = "删除诊断（支持批量）")
    @DeleteMapping("/remove-batch")
    public JsonVO<String> removeBatch(@RequestBody List<String> ids) {
        return null;
    }
}
