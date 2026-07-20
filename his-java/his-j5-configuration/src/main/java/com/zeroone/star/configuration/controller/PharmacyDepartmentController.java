package com.zeroone.star.configuration.controller;

import com.baomidou.mybatisplus.core.metadata.IPage;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.configuration.entity.CusServiceLocationTemplate;
import com.zeroone.star.configuration.service.ICusServiceLocationTemplateService;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.tags.Tag;
import org.springframework.web.bind.annotation.*;
import org.springframework.beans.factory.annotation.Autowired;

import java.util.HashMap;
import java.util.Map;

@RestController
@RequestMapping("/config/pharmacyDepartment")
@Tag(name = "药品科室配置", description = "药品科室项目配置相关接口")
public class PharmacyDepartmentController {

    @Autowired
    private ICusServiceLocationTemplateService serviceLocationTemplateService;

    @GetMapping("/list")
    @Operation(summary = "获取科室项目列表", description = "根据条件分页查询药品科室项目配置列表")
    public JsonVO<IPage<CusServiceLocationTemplate>> list(
            @Parameter(description = "页码，从1开始") @RequestParam(defaultValue = "1") long current,
            @Parameter(description = "每页数量") @RequestParam(defaultValue = "10") long size,
            @Parameter(description = "机构标识") @RequestParam(required = false) String organizationId,
            @Parameter(description = "项table") @RequestParam(required = false) String itemTable,
            @Parameter(description = "分发类别编码") @RequestParam(required = false) String distributionCategoryCode,
            @Parameter(description = "默认位置标识") @RequestParam(required = false) String defLocationId,
            @Parameter(description = "项定义标识") @RequestParam(required = false) String itemDefinitionId) {
        
        Page<CusServiceLocationTemplate> page = new Page<>(current, size);
        Map<String, Object> params = new HashMap<>();
        params.put("organizationId", organizationId);
        params.put("itemTable", itemTable);
        params.put("distributionCategoryCode", distributionCategoryCode);
        params.put("defLocationId", defLocationId);
        params.put("itemDefinitionId", itemDefinitionId);
        
        IPage<CusServiceLocationTemplate> result = serviceLocationTemplateService.listPage(page, params);
        return JsonVO.success(result);
    }
    
    @PostMapping("/save")
    @Operation(summary = "保存科室项目", description = "新增或更新药品科室项目配置")
    public JsonVO<Boolean> save(
            @Parameter(description = "科室项目配置实体") @RequestBody CusServiceLocationTemplate template) {
        boolean result = serviceLocationTemplateService.saveTemplate(template);
        return JsonVO.success(result);
    }
}