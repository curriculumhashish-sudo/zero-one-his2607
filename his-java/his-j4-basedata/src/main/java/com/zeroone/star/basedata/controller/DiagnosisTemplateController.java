package com.zeroone.star.basedata.controller;

import com.zeroone.star.basedata.service.ICusConditionTemplateService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j4.diagnosis.DiagnosisTemplateDTO;
import com.zeroone.star.project.j4.basedata.DiagnosisApis;
import com.zeroone.star.project.query.j4.diagnosis.DiagnosisTemplateQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j4.diagnosis.DiagnosisTemplateVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import org.springframework.web.bind.annotation.*;

import java.util.List;

/**
 * <p>
 * 描述：诊断模板控制器
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@RestController
@Tag(name = "diagnosis-template", description = "诊断模板管理")
public class DiagnosisTemplateController implements DiagnosisApis {

    @Resource
    private ICusConditionTemplateService templateService;

    @Operation(summary = "分页查询诊断模板")
    @PostMapping("query-template")
    @Override
    public JsonVO<PageDTO<DiagnosisTemplateVO>> queryTemplates(@RequestBody DiagnosisTemplateQuery query) {
        try {
            List<DiagnosisTemplateVO> rows = templateService.pageQuery(query);
            PageDTO<DiagnosisTemplateVO> pageResult = new PageDTO<>();
            pageResult.setRows(rows);
            pageResult.setPageIndex(query.getPageIndex());
            pageResult.setPageSize(query.getPageSize());
            pageResult.setTotal((long) rows.size());
            pageResult.setPages(rows.isEmpty() ? 0L
                    : (long) Math.ceil((double) rows.size() / query.getPageSize()));
            return JsonVO.success(pageResult);
        } catch (Exception e) {
            return JsonVO.create(null, 500, e.getMessage());
        }
    }

    @Operation(summary = "新增诊断模板")
    @PostMapping("add-template")
    @Override
    public JsonVO<String> addTemplate(@RequestBody DiagnosisTemplateDTO dto) {
        try {
            String id = templateService.saveTemplate(dto);
            return JsonVO.success(id);
        } catch (Exception e) {
            return JsonVO.create(null, 500, e.getMessage());
        }
    }

    @Operation(summary = "编辑诊断模板")
    @PutMapping("modify-template")
    @Override
    public JsonVO<String> modifyTemplate(@RequestBody DiagnosisTemplateDTO dto) {
        try {
            templateService.modifyTemplate(dto);
            return JsonVO.success("编辑成功");
        } catch (Exception e) {
            return JsonVO.create(null, 500, e.getMessage());
        }
    }

    @Operation(summary = "删除诊断模板")
    @DeleteMapping("delete-template/{id}")
    @Override
    public JsonVO<String> deleteTemplate(@PathVariable String id) {
        try {
            templateService.deleteTemplate(id);
            return JsonVO.success("删除成功");
        } catch (Exception e) {
            return JsonVO.create(null, 500, e.getMessage());
        }
    }
}
