package com.zeroone.star.configuration.controller;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j5.bindingMapping.TreatmentBindingDTO;
import com.zeroone.star.project.dto.j5.bindingMapping.UsageBindingDTO;
import com.zeroone.star.project.j5.bindingMapping.BindingMappingApis;
import com.zeroone.star.project.query.j5.bindingMapping.TreatmentBindingQuery;
import com.zeroone.star.project.query.j5.bindingMapping.UsageBindingQuery;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.validation.Valid;
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
@RequestMapping("/config/binding-mapping")
@Tag(name = "配置管理-绑定映射", description = "诊疗/用法绑定映射接口")
public class BindingMappingController implements BindingMappingApis {

    @Override
    @Operation(summary = "查询诊疗绑定列表")
    @GetMapping("/query-all-treatment-bindings")
    public JsonVO<PageDTO<TreatmentBindingDTO>> queryAllTreatmentBindings(@Valid TreatmentBindingQuery query) {
        // TODO: 调用 service 层实现
        return JsonVO.success(null);
    }

    @Override
    @Operation(summary = "查询用法绑定列表")
    @GetMapping("/query-all-usage-bindings")
    public JsonVO<PageDTO<UsageBindingDTO>> queryAllUsageBindings(@Valid UsageBindingQuery query) {
        // TODO: 调用 service 层实现
        return JsonVO.success(null);
    }

    @Override
    @Operation(summary = "添加诊疗绑定")
    @PostMapping("/add-treatment-binding")
    public JsonVO<String> addTreatmentBinding(@Valid @RequestBody TreatmentBindingDTO data) {
        // TODO: 调用 service 层实现
        return JsonVO.success("添加成功");
    }

    @Override
    @Operation(summary = "添加用法绑定")
    @PostMapping("/add-usage-binding")
    public JsonVO<String> addUsageBinding(@Valid @RequestBody UsageBindingDTO data) {
        // TODO: 调用 service 层实现
        return JsonVO.success("添加成功");
    }

    @Override
    @Operation(summary = "编辑诊疗绑定")
    @PutMapping("/modify-treatment-binding")
    public JsonVO<String> modifyTreatmentBinding(@Valid @RequestBody TreatmentBindingDTO data) {
        // TODO: 调用 service 层实现
        return JsonVO.success("编辑成功");
    }

    @Override
    @Operation(summary = "编辑用法绑定")
    @PutMapping("/modify-usage-binding")
    public JsonVO<String> modifyUsageBinding(@Valid @RequestBody UsageBindingDTO data) {
        // TODO: 调用 service 层实现
        return JsonVO.success("编辑成功");
    }

    @Override
    @Operation(summary = "删除诊疗绑定")
    @DeleteMapping("/delete-treatment-binding/{id}")
    public JsonVO<String> deleteTreatmentBinding(@PathVariable("id") String id) {
        // TODO: 调用 service 层实现
        return JsonVO.success("删除成功");
    }

    @Override
    @Operation(summary = "删除用法绑定")
    @DeleteMapping("/delete-usage-binding/{id}")
    public JsonVO<String> deleteUsageBinding(@PathVariable("id") String id) {
        // TODO: 调用 service 层实现
        return JsonVO.success("删除成功");
    }
}
