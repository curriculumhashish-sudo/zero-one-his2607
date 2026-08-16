package com.zeroone.star.configuration.controller;

import com.zeroone.star.configuration.service.IEmrTemplateService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.j5.emr.MedicalTemplateApis;
import com.zeroone.star.project.query.j5.emr.MedicalTemplateQuery;
import com.zeroone.star.project.vo.j5.emr.MedicalTemplateDetailVO;
import com.zeroone.star.project.vo.j5.emr.MedicalTemplateVO;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import org.springframework.web.bind.annotation.*;

/**
 * 病历模板控制器
 */
@Tag(name = "配置管理-病历模板", description = "病历模板配置管理接口")
@RestController
public class MedicalTemplateController implements MedicalTemplateApis {

    @Resource
    private IEmrTemplateService emrTemplateService;

    @Override
    public JsonVO<PageDTO<MedicalTemplateVO>> getList(MedicalTemplateQuery query) {
        // 参数校验：分页默认值
        if (query.getPageIndex() < 1) {
            query.setPageIndex(1);
        }
        if (query.getPageSize() < 1 || query.getPageSize() > 200) {
            query.setPageSize(10);
        }

        PageDTO<MedicalTemplateVO> pageDTO = emrTemplateService.listTemplates(query);
        return JsonVO.success(pageDTO);
    }

    @Override
    public JsonVO<MedicalTemplateDetailVO> getDetail(
            @Parameter(description = "模板ID", required = true, example = "1816800000000000001")
            @PathVariable("id") String id) {
        if (id == null || id.isBlank()) {
            return JsonVO.fail("模板ID不能为空");
        }

        MedicalTemplateDetailVO detail = emrTemplateService.getTemplateDetail(id);
        return JsonVO.success(detail);
    }
}
