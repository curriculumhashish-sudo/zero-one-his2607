package com.zeroone.star.configuration.controller;

import com.zeroone.star.configuration.service.BindingMappingService;
import com.zeroone.star.configuration.service.impl.BindingMappingServiceImpl;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j5.bindingMapping.BindingInitDTO;
import com.zeroone.star.project.dto.j5.bindingMapping.TreatmentBindingDTO;
import com.zeroone.star.project.j5.bindingMapping.BindingMappingApis;
import com.zeroone.star.project.query.j5.bindingMapping.TreatmentBindingQuery;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.validation.Valid;
import lombok.RequiredArgsConstructor;
import org.springframework.web.bind.annotation.*;

/**
 * <p>
 * 描述：诊疗/用法绑定映射控制器
 * </p>
 *
 * @author 扶舒
 * @version 1.0.0
 */
@RestController
@RequestMapping("/binding-mapping")
@Tag(name = "配置管理-整料耗材绑定", description = "诊疗/用法绑定映射接口")
@RequiredArgsConstructor
public class BindingMappingController implements BindingMappingApis {

    private final BindingMappingService bindingMappingService;
    private final BindingMappingServiceImpl bindingMappingServiceImpl;

    @Override
    @Operation(summary = "初始化", description = "返回绑定映射页面初始化数据（下拉选项等）")
    @GetMapping("/init")
    public JsonVO<BindingInitDTO> init() {
        return JsonVO.success(bindingMappingServiceImpl.initBindingOptions());
    }

    @Override
    @Operation(summary = "获取绑定数据", description = "条件+分页查询诊疗绑定信息")
    @GetMapping("/treatment/page")
    public JsonVO<PageDTO<TreatmentBindingDTO>> queryAllTreatmentBindings(TreatmentBindingQuery query) {
        return bindingMappingService.queryAllTreatmentBindings(query);
    }

    @Override
    @Operation(summary = "添加绑定", description = "添加诊疗绑定信息")
    @PostMapping("/treatment")
    public JsonVO<String> addTreatmentBinding(@Valid @RequestBody TreatmentBindingDTO data) {
        return bindingMappingService.addTreatmentBinding(data);
    }

    @Override
    @Operation(summary = "修改绑定", description = "修改诊疗绑定信息")
    @PutMapping("/treatment")
    public JsonVO<String> modifyTreatmentBinding(@Valid @RequestBody TreatmentBindingDTO data) {
        return bindingMappingService.modifyTreatmentBinding(data);
    }

    @Override
    @Operation(summary = "删除绑定", description = "删除诊疗绑定信息")
    @DeleteMapping("/treatment/{id}")
    public JsonVO<String> deleteTreatmentBinding(@PathVariable String id) {
        return bindingMappingService.deleteTreatmentBinding(id);
    }

}
