package com.zeroone.star.j3.supplier.controller;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j3.supplier.SupplierBatchDTO;
import com.zeroone.star.project.dto.j3.supplier.SupplierFullDTO;
import com.zeroone.star.project.j3.supplier.SupplierApis;
import com.zeroone.star.project.query.j3.supplier.SupplierQuery;
import com.zeroone.star.project.vo.j3.supplier.SupplierDetailVO;
import com.zeroone.star.project.vo.j3.supplier.SupplierListVO;
import com.zeroone.star.project.vo.j3.supplier.SupplierNameVO;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
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

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

/**
 * <p>
 * 描述：供应商管理 Controller —
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author zzcsz
 * @version 1.0.0
 */
@RestController
@RequestMapping("/suppliers")
@Tag(name = "供应商管理", description = "供应商基础信息查询接口 — 下拉选择 / 列表分页 / 详情回填")
public class SupplierController implements SupplierApis {

    /** 模拟有效供应商ID */
    private static final List<String> VALID_IDS = Arrays.asList("1", "2", "3", "4", "5");

    /** 模拟现有信用代码（新增时不可重复） */
    private static final List<String> EXISTING_CODES = Arrays.asList(
            "91110000100000AAAA", "91110000100000BBBB",
            "91110000100000CCCC", "91110000100000DDDD", "91110000100000EEEE"
    );

    /** 模拟有关联采购单不可删除的供应商ID */
    private static final List<String> REFERENCED_FOR_DELETE = Arrays.asList("1", "2");

    /** 模拟有未完成采购单不可停用的供应商ID */
    private static final List<String> REFERENCED_FOR_DISABLE = Arrays.asList("1");

    /**
     * <p>获取供应商名称列表（用于下拉框）</p>
     * <p>业务场景：采购单选择供应商、入库单选择供应商、筛选条件供应商下拉、搜索框自动补全</p>
     */
    @Override
    @GetMapping("/names")
    @Operation(summary = "获取供应商名称列表",
               description = "获取供应商名称，支持按状态过滤启用供应商，用于下拉框选择。仅返回 id 和 name，无分页。")
    public JsonVO<List<SupplierNameVO>> querySupplierNames(
            @Parameter(description = "状态枚举值（ENABLED-启用 / DISABLED-停用），不传则获取全部",
                       example = "ENABLED")
            @RequestParam(required = false) String statusEnum) {

        // ---- 模拟业务校验：非法状态枚举值 ----
        if (statusEnum != null
                && !"ENABLED".equals(statusEnum)
                && !"DISABLED".equals(statusEnum)) {
            return JsonVO.fail("statusEnum 取值非法，仅支持 ENABLED 或 DISABLED");
        }

        // ---- 模拟数据：构造多条供应商下拉数据 ----
        List<SupplierNameVO> mockList = Arrays.asList(
                new SupplierNameVO("1001", "华润医药集团有限公司"),
                new SupplierNameVO("1002", "国药控股股份有限公司"),
                new SupplierNameVO("1003", "九州通医药集团股份有限公司"),
                new SupplierNameVO("1004", "上海医药集团股份有限公司"),
                new SupplierNameVO("1005", "广州白云山医药集团股份有限公司")
        );

        // ---- 模拟按状态过滤 ----
        if ("DISABLED".equals(statusEnum)) {
            // 模拟已停用供应商（前端一般只请求启用，此分支仅作自测演示）
            List<SupplierNameVO> disabledList = Arrays.asList(
                    new SupplierNameVO("2001", "已停用供应商A"),
                    new SupplierNameVO("2002", "已停用供应商B")
            );
            return JsonVO.success(disabledList);
        }

        return JsonVO.success(mockList);
    }

    /**
     * <p>获取供应商列表（条件+分页）</p>
     * <p>业务场景：供应商管理列表页、选择供应商弹窗中的表格展示</p>
     */
    @Override
    @GetMapping("/list")
    @Operation(summary = "获取供应商列表（条件+分页）",
               description = "支持按名称（模糊）、信用代码、状态、类型筛选，返回分页结果。列表仅展示关键字段，完整资质请走详情接口。")
    public JsonVO<PageDTO<SupplierListVO>> querySupplierList(
            @Parameter(description = "查询条件（含分页参数 pageIndex/pageSize + 筛选字段 name/creditCode/statusEnum/typeEnum）",
                       required = true)
            @Valid SupplierQuery query) {

        // ---- 构造 Mock 列表数据 ----
        List<SupplierListVO> allMock = Arrays.asList(
                new SupplierListVO("1001", "SUP20250001", "华润医药集团有限公司",
                        "ENTERPRISE", "91110000100000AAAA", "张三",
                        "010-88888888", "ENABLED", 1, "2025-07-18 10:00:00"),
                new SupplierListVO("1002", "SUP20250002", "国药控股股份有限公司",
                        "ENTERPRISE", "91110000100000BBBB", "李四",
                        "021-66666666", "ENABLED", 2, "2025-07-17 15:30:00"),
                new SupplierListVO("1003", "SUP20250003", "九州通医药集团股份有限公司",
                        "ENTERPRISE", "91110000100000CCCC", "王五",
                        "027-55555555", "ENABLED", 3, "2025-07-16 09:00:00"),
                new SupplierListVO("1004", "SUP20250004", "上海医药集团股份有限公司",
                        "ENTERPRISE", "91110000100000DDDD", "赵六",
                        "021-33333333", "ENABLED", 4, "2025-07-15 14:00:00"),
                new SupplierListVO("1005", "SUP20250005", "广州白云山医药集团股份有限公司",
                        "ENTERPRISE", "91110000100000EEEE", "孙七",
                        "020-22222222", "DISABLED", 5, "2025-06-01 08:00:00")
        );

        // ---- 模拟筛选：名称模糊匹配 ----
        List<SupplierListVO> filtered = new ArrayList<>();
        if (query.getName() != null && !query.getName().isEmpty()) {
            for (SupplierListVO vo : allMock) {
                if (vo.getName().contains(query.getName())) {
                    filtered.add(vo);
                }
            }
        } else {
            filtered = new ArrayList<>(allMock);
        }

        // ---- 模拟筛选：信用代码精确匹配 ----
        if (query.getCreditCode() != null && !query.getCreditCode().isEmpty()) {
            filtered.removeIf(vo -> !vo.getCreditCode().equals(query.getCreditCode()));
        }

        // ---- 模拟筛选：状态 ----
        if (query.getStatusEnum() != null && !query.getStatusEnum().isEmpty()) {
            filtered.removeIf(vo -> !vo.getStatusEnum().equals(query.getStatusEnum()));
        }

        // ---- 模拟筛选：类型 ----
        if (query.getTypeEnum() != null && !query.getTypeEnum().isEmpty()) {
            filtered.removeIf(vo -> !vo.getTypeEnum().equals(query.getTypeEnum()));
        }

        // ---- 模拟分页 ----
        long pageIndex = Math.max(query.getPageIndex(), 1);
        long pageSize  = Math.max(query.getPageSize(), 1);
        long total     = filtered.size();
        long pages     = (total + pageSize - 1) / pageSize;

        int fromIndex = (int) ((pageIndex - 1) * pageSize);
        int toIndex   = (int) Math.min(fromIndex + pageSize, total);

        List<SupplierListVO> pageRows = (fromIndex < total)
                ? filtered.subList(fromIndex, toIndex)
                : new ArrayList<>();

        // ---- 构造分页返回体 ----
        PageDTO<SupplierListVO> pageDTO = new PageDTO<>();
        pageDTO.setPageIndex(pageIndex);
        pageDTO.setPageSize(pageSize);
        pageDTO.setTotal(total);
        pageDTO.setPages(pages);
        pageDTO.setRows(pageRows);

        return JsonVO.success(pageDTO);
    }


    /**
     * <p>获取供应商详情</p>
     * <p>业务场景：编辑供应商表单回填、供应商详情弹窗、审核页面查看完整资质信息</p>
     */
    @Override
    @GetMapping("/detail")
    @Operation(summary = "获取供应商详情",
               description = "根据ID查询单条供应商的完整资质信息（含统一信用代码、法人、开户行、银行账户等），用于编辑表单回填和详情弹窗。")
    public JsonVO<SupplierDetailVO> querySupplierDetail(
            @Parameter(description = "供应商主键ID",
                       example = "1001",
                       required = true)
            @RequestParam(required = true) String id) {

        // ---- 模拟异常：id 为空 ----
        if (id == null || id.trim().isEmpty()) {
            return JsonVO.fail("供应商ID不能为空");
        }

        // ---- 模拟不存在：只有 1001~1005 为有效数据 ----
        List<String> validIds = Arrays.asList("1001", "1002", "1003", "1004", "1005");
        if (!validIds.contains(id.trim())) {
            return JsonVO.fail("供应商不存在，请检查ID是否正确");
        }

        // ---- 根据 id 返回不同的模拟详情 ----
        SupplierDetailVO detail;
        switch (id.trim()) {
            case "1001":
                detail = new SupplierDetailVO(
                        "1001", "SUP20250001", "华润医药集团有限公司",
                        "ENTERPRISE", "91110000100000AAAA", "张三",
                        "北京市朝阳区建国路88号", "HRYY", "WXDG",
                        "010-88888888", "contact@huarun.com",
                        "中国工商银行北京分行", "6222000000000000001",
                        "ORG001", 1, "ENABLED",
                        "央企背景，年合作额超千万，优质核心供应商");
                break;
            case "1002":
                detail = new SupplierDetailVO(
                        "1002", "SUP20250002", "国药控股股份有限公司",
                        "ENTERPRISE", "91110000100000BBBB", "李四",
                        "上海市浦东新区张江路168号", "GYKG", "WXDG",
                        "021-66666666", "contact@sinopharm.com",
                        "中国建设银行上海分行", "6222000000000000002",
                        "ORG001", 2, "ENABLED",
                        "上市公司，覆盖全国冷链配送网络");
                break;
            case "1003":
                detail = new SupplierDetailVO(
                        "1003", "SUP20250003", "九州通医药集团股份有限公司",
                        "ENTERPRISE", "91110000100000CCCC", "王五",
                        "武汉市汉阳区龙阳大道特8号", "JZTYY", "WXDG",
                        "027-55555555", "contact@jztey.com",
                        "中国农业银行武汉分行", "6222000000000000003",
                        "ORG001", 3, "ENABLED",
                        "民营龙头，药品批发覆盖率全国第一");
                break;
            case "1004":
                detail = new SupplierDetailVO(
                        "1004", "SUP20250004", "上海医药集团股份有限公司",
                        "ENTERPRISE", "91110000100000DDDD", "赵六",
                        "上海市黄浦区南京东路100号", "SHYY", "WXDG",
                        "021-33333333", "contact@shapharma.com",
                        "交通银行上海分行", "6222000000000000004",
                        "ORG001", 4, "ENABLED",
                        "沪港两地上市，综合型医药产业集团");
                break;
            default: // 1005
                detail = new SupplierDetailVO(
                        "1005", "SUP20250005", "广州白云山医药集团股份有限公司",
                        "ENTERPRISE", "91110000100000EEEE", "孙七",
                        "广州市白云区同和街同泰路1051号", "GZBYS", "WXDG",
                        "020-22222222", "contact@bys.com",
                        "中国银行广州分行", "6222000000000000005",
                        "ORG001", 5, "DISABLED",
                        "因年度评审未达标，暂已停用");
                break;
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