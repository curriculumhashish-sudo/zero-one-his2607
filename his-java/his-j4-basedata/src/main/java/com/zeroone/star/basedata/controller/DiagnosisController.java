package com.zeroone.star.basedata.controller;

import com.zeroone.star.basedata.service.IDiagnosisService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j4.diagnosis.DiagnosisDefinitionDTO;
import com.zeroone.star.project.dto.j4.diagnosis.DiagnosisSaveDTO;
import com.zeroone.star.project.dto.j4.diagnosis.DiagnosisUpdateDTO;
import com.zeroone.star.project.dto.j4.diagnosis.DiagnosisIdsDTO;
import com.zeroone.star.project.dto.j4.diagnosis.DiagnosisStatusDTO;
import com.zeroone.star.project.j4.basedata.DiagnosisApis;
import com.zeroone.star.project.query.j4.diagnosis.DiagnosisPageQuery;
import com.zeroone.star.project.query.j4.diagnosis.DiagnosisSelectPageQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j4.diagnosis.DiagnosisDetailVO;
import com.zeroone.star.project.vo.j4.diagnosis.DiagnosisListVO;
import com.zeroone.star.project.vo.j4.diagnosis.DiagnosisOptionVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import jakarta.validation.Valid;
import jakarta.validation.constraints.NotBlank;
import org.springframework.validation.annotation.Validated;
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
@Validated
@RestController
@RequestMapping("/diagnosis")
@Tag(name = "诊断管理", description = "诊断管理接口")
public class DiagnosisController implements DiagnosisApis {

    @Resource
    private IDiagnosisService definitionService;

    @Override
    @Operation(summary = "获取选择诊断列表（条件+分页）")
    @GetMapping("/query-select-list")
    public JsonVO<PageDTO<DiagnosisOptionVO>> queryDiagnosisSelectList(
            @Valid DiagnosisSelectPageQuery query) {
        return JsonVO.success(definitionService.queryDiagnosisSelectList(query));
    }

    @Override
    @Operation(summary = "获取诊断列表（条件+分页）")
    @GetMapping("/query-list")
    public JsonVO<PageDTO<DiagnosisListVO>> queryDiagnosisList(@Valid DiagnosisPageQuery query) {
        return JsonVO.success(definitionService.queryDiagnosisList(query));
    }

    @Override
    @Operation(summary = "获取诊断详情")
    @GetMapping("/get-detail/{id}")
    public JsonVO<DiagnosisDetailVO> queryDiagnosisDetail(@PathVariable @NotBlank(message = "诊断ID不能为空") String id) {
        return JsonVO.success(definitionService.queryDiagnosisDetail(id));
    }

    @Operation(summary = "新增诊断")
    @PostMapping("/add-definition")
    @Override
    public JsonVO<String> addDefinition(@Valid @RequestBody DiagnosisSaveDTO dto) {
        return null;
    }

    @Operation(summary = "修改诊断")
    @PutMapping("/modify-definition")
    @Override
    public JsonVO<String> modifyDefinition(@Valid @RequestBody DiagnosisUpdateDTO dto) {
        return null;
    }

    @Operation(summary = "启/停用诊断（支持批量）")
    @PutMapping("/modify-batch-status")
    @Override
    public JsonVO<String> modifyBatchStatus(@Valid @RequestBody DiagnosisStatusDTO dto) {
        return null;
    }

    @Operation(summary = "删除诊断（支持批量）")
    @DeleteMapping("/remove-batch")
    @Override
    public JsonVO<String> removeBatch(@Valid @RequestBody DiagnosisIdsDTO dto) {
        return null;
    }
}
