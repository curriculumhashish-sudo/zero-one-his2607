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

    /**
     * <p>新增供应商</p>
     * <p>业务场景：供应商管理页面新增按钮、批量导入后的逐条新增</p>
     */
    @Override
    @PostMapping("/add")
    @Operation(summary = "新增供应商",
               description = "新增一条供应商记录，包含信用代码、联系方式、资质等信息")
    public JsonVO<String> addSupplier(
            @Parameter(description = "供应商数据", required = true)
            @Valid @RequestBody SupplierFullDTO dto) {

        if (dto.getName() == null || dto.getName().trim().isEmpty()) {
            return JsonVO.fail("供应商名称不能为空");
        }
        if (dto.getCreditCode() == null || dto.getCreditCode().trim().isEmpty()) {
            return JsonVO.fail("统一信用代码不能为空");
        }
        if (dto.getPhone() == null || dto.getPhone().trim().isEmpty()) {
            return JsonVO.fail("联系电话不能为空");
        }
        if (!dto.getCreditCode().matches("^[0-9A-Z]{18}$")) {
            return JsonVO.fail("统一信用代码格式不正确，应为18位数字或大写字母");
        }
        if (EXISTING_CODES.contains(dto.getCreditCode())) {
            return JsonVO.fail("统一信用代码已存在，请检查是否重复录入");
        }

        String supplierCode = "SUP" + System.currentTimeMillis();
        return JsonVO.success("新增供应商成功，供应商编号：" + supplierCode);
    }

    /**
     * <p>修改供应商</p>
     * <p>业务场景：供应商管理页面编辑按钮、供应商详情页修改信息</p>
     */
    @Override
    @PutMapping("/update/{id}")
    @Operation(summary = "修改供应商",
               description = "根据ID修改供应商信息，部分关键字段有限制")
    public JsonVO<String> updateSupplier(
            @Parameter(description = "供应商ID", example = "1", required = true)
            @PathVariable String id,
            @Parameter(description = "供应商数据", required = true)
            @Valid @RequestBody SupplierFullDTO dto) {

        if (id == null || id.trim().isEmpty()) {
            return JsonVO.fail("供应商ID不能为空");
        }
        if (!VALID_IDS.contains(id.trim())) {
            return JsonVO.fail("供应商不存在，请检查ID是否正确");
        }
        if (dto.getName() == null || dto.getName().trim().isEmpty()) {
            return JsonVO.fail("供应商名称不能为空");
        }
        if (dto.getCreditCode() != null && !dto.getCreditCode().equals(EXISTING_CODES.get(0))) {
            return JsonVO.fail("统一信用代码不允许直接修改，如需修改请联系管理员");
        }

        return JsonVO.success("修改供应商成功");
    }

    /**
     * <p>删除供应商</p>
     * <p>业务场景：供应商管理页面删除按钮、供应商详情页删除操作</p>
     */
    @Override
    @DeleteMapping("/delete/{id}")
    @Operation(summary = "删除供应商",
               description = "根据ID删除供应商记录，需检查是否被采购单引用")
    public JsonVO<String> deleteSupplier(
            @Parameter(description = "供应商ID", example = "1", required = true)
            @PathVariable String id) {

        if (id == null || id.trim().isEmpty()) {
            return JsonVO.fail("供应商ID不能为空");
        }
        if (!VALID_IDS.contains(id.trim())) {
            return JsonVO.fail("供应商不存在，请检查ID是否正确");
        }
        if (REFERENCED_FOR_DELETE.contains(id.trim())) {
            return JsonVO.fail("该供应商存在关联的采购单记录，无法删除，请先处理相关业务");
        }

        return JsonVO.success("删除供应商成功");
    }

    /**
     * <p>批量启/停用供应商</p>
     * <p>业务场景：供应商管理页面批量操作、供应商状态批量调整</p>
     */
    @Override
    @PutMapping("/batchUpdateStatus")
    @Operation(summary = "批量启/停用供应商",
               description = "支持批量操作供应商的启用状态，影响采购业务")
    public JsonVO<String> batchUpdateSuppliers(
            @Parameter(description = "批量操作参数", required = true)
            @Valid @RequestBody SupplierBatchDTO batchDTO) {

        if (batchDTO.getIds() == null || batchDTO.getIds().isEmpty()) {
            return JsonVO.fail("请选择需要操作的供应商");
        }

        String status = batchDTO.getStatusEnum();
        if (status == null || (!"ENABLED".equals(status) && !"DISABLED".equals(status))) {
            return JsonVO.fail("状态枚举值非法，仅支持 ENABLED 或 DISABLED");
        }

        List<String> invalidIds = new ArrayList<>();
        for (String id : batchDTO.getIds()) {
            if (!VALID_IDS.contains(id)) {
                invalidIds.add(id);
            }
        }
        if (!invalidIds.isEmpty()) {
            return JsonVO.fail("以下供应商ID不存在：" + String.join(", ", invalidIds));
        }

        if ("DISABLED".equals(status)) {
            for (String id : batchDTO.getIds()) {
                if (REFERENCED_FOR_DISABLE.contains(id)) {
                    return JsonVO.fail("供应商ID " + id + " 存在未完成的采购单，无法停用");
                }
            }
        }

        String statusText = "ENABLED".equals(status) ? "启用" : "停用";
        return JsonVO.success("成功批量" + statusText + " " + batchDTO.getIds().size() + " 个供应商");
    }

}
