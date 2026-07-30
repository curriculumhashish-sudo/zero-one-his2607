package com.zeroone.star.j3.supplier.controller;

import com.zeroone.star.j3.supplier.service.ISupplierService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j3.supplier.SupplierBatchDTO;
import com.zeroone.star.project.dto.j3.supplier.SupplierFullDTO;
import com.zeroone.star.project.j3.supplier.SupplierApis;
import com.zeroone.star.project.query.j3.supplier.SupplierQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j3.supplier.SupplierDetailVO;
import com.zeroone.star.project.vo.j3.supplier.SupplierListVO;
import com.zeroone.star.project.vo.j3.supplier.SupplierNameVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import jakarta.validation.Valid;
import org.springframework.util.StringUtils;
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

@RestController
@RequestMapping("/suppliers")
@Tag(name = "供应商管理", description = "供应商基础信息管理接口")
public class SupplierController implements SupplierApis {

    @Resource
    private ISupplierService supplierService;

    @Override
    @GetMapping("/names")
    @Operation(summary = "获取供应商名称", description = "用于下拉框或业务选择框，默认只返回启用且未删除的供应商，支持按名称、拼音码、五笔码搜索")
    public JsonVO<List<SupplierNameVO>> querySupplierNames(
            @Parameter(description = "状态枚举：ENABLED-启用，DISABLED-停用；不传默认查询启用供应商")
            @RequestParam(required = false) String statusEnum,
            @Parameter(description = "搜索关键字，支持供应商名称、拼音码、五笔码模糊匹配")
            @RequestParam(required = false) String keyword) {
        try {
            return JsonVO.success(supplierService.querySupplierNames(statusEnum, keyword));
        } catch (IllegalArgumentException e) {
            return JsonVO.fail(e.getMessage());
        }
    }

    @Override
    @GetMapping("/list")
    @Operation(summary = "获取供应商列表", description = "分页查询供应商管理列表，支持按关键字、名称、业务编号、信用代码、类型、状态、机构、拼音码、五笔码、电话筛选")
    public JsonVO<PageDTO<SupplierListVO>> querySupplierList(
            @Parameter(description = "供应商查询条件，包含分页参数和筛选字段")
            @Valid SupplierQuery query) {
        try {
            return JsonVO.success(supplierService.querySupplierList(query));
        } catch (IllegalArgumentException e) {
            return JsonVO.fail(e.getMessage());
        }
    }

    @Override
    @GetMapping("/detail")
    @Operation(summary = "获取供应商详情", description = "根据供应商ID查询单条完整信息，用于详情查看和编辑表单回显，已删除供应商不会返回")
    public JsonVO<SupplierDetailVO> querySupplierDetail(
            @Parameter(description = "供应商主键ID", required = true)
            @RequestParam String id) {
        if (!StringUtils.hasText(id)) {
            return JsonVO.fail("供应商ID不能为空");
        }
        SupplierDetailVO detail = supplierService.querySupplierDetail(id);
        if (detail == null) {
            return JsonVO.fail("供应商不存在");
        }
        return JsonVO.success(detail);
    }


    @Override
    @PostMapping("/add")
    @Operation(summary = "新增供应商",
               description = "新增一条供应商记录，包含信用代码、联系方式、资质等信息")
    public JsonVO<String> addSupplier(
            @Parameter(description = "供应商数据", required = true)
            @Valid @RequestBody SupplierFullDTO dto) {
        try {
            String result = supplierService.addSupplier(dto);
            if (result != null){
                return JsonVO.success(result);
            }
            return JsonVO.fail("新增供应商失败");
        } catch (IllegalArgumentException e) {
            return JsonVO.fail(e.getMessage());
        }
    }

    @Override
    @PutMapping("/update/{id}")
    @Operation(summary = "修改供应商",
               description = "根据ID修改供应商信息，部分关键字段有限制")
    public JsonVO<String> updateSupplier(
            @Parameter(description = "供应商ID", example = "4100000000000000002", required = true)
            @PathVariable String id,
            @Parameter(description = "供应商数据", required = true)
            @Valid  SupplierFullDTO dto) {
        try {
            String result = supplierService.updateSupplier(id, dto);
            if(result!=null){
                return JsonVO.success(result);
            }
            return JsonVO.fail("修改供应商失败");
        } catch (IllegalArgumentException e) {
            return JsonVO.fail(e.getMessage());
        }
    }

    @Override
    @DeleteMapping("/delete/{id}")
    @Operation(summary = "删除供应商",
               description = "根据ID删除供应商记录（软删除）")
    public JsonVO<String> deleteSupplier(
            @Parameter(description = "供应商ID", example = "4100000000000000002", required = true)
            @PathVariable String id) {
        try {
            String result = supplierService.deleteSupplier(id);
            if(result!=null){
                return JsonVO.success(result);
            }
            return JsonVO.fail("删除供应商失败");
        } catch (IllegalArgumentException e) {
            return JsonVO.fail(e.getMessage());
        }
    }

    @Override
    @PutMapping("/batchUpdateStatus")
    @Operation(summary = "批量启/停用供应商",
               description = "支持批量操作供应商的启用状态，影响采购业务")
    public JsonVO<String> batchUpdateSuppliers(
            @Parameter(description = "批量操作参数", required = true)
            @Valid  @RequestBody SupplierBatchDTO batchDTO) {
        try {
            String result = supplierService.batchUpdateSuppliers(batchDTO);
            return JsonVO.success(result);
        } catch (IllegalArgumentException e) {
            return JsonVO.fail(e.getMessage());
        }
    }
}
