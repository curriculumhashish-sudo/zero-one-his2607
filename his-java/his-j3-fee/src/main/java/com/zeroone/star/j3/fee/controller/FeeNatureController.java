package com.zeroone.star.j3.fee.controller;

import com.zeroone.star.j3.fee.service.IFeeNatureService;
import com.zeroone.star.project.dto.j3.fee.FeeNatureDTO;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.j3.fee.FeeNatureApis;
import com.zeroone.star.project.query.j3.fee.FeeNatureQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j3.fee.FeeNatureVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import jakarta.validation.Valid;
import org.springframework.web.bind.annotation.*;

import java.util.ArrayList;
import java.util.List;

/**
 * <p>
 * 描述：费用性质管理控制器
 * </p>
 * @author 白文儿
 * @version 1.0.0
 */
@RestController
@RequestMapping("/fee-natures")
@Tag(name = "费用性质管理", description = "费用性质相关操作接口")
public class FeeNatureController implements FeeNatureApis {

    @Resource
    private IFeeNatureService feeNatureService;

    @Override
    @GetMapping("/page")
    @Operation(summary = "获取费用性质列表（条件+分页）",
            description = "查询所有费用性质记录，收费结算模块引用此列表生成患者费用明细。支持按名称或编码模糊搜索，返回分页结果")
    public JsonVO<PageDTO<FeeNatureVO>> getPageList(@Valid FeeNatureQuery query) {
        List<FeeNatureVO> list = feeNatureService.listFeeNatures(query);
        PageDTO<FeeNatureVO> pageDTO = new PageDTO<>();
        pageDTO.setPageIndex(query.getPageIndex());
        pageDTO.setPageSize(query.getPageSize());
        pageDTO.setTotal((long) list.size());
        pageDTO.setPages(list.isEmpty() ? 0L : (list.size() + query.getPageSize() - 1) / query.getPageSize());
        pageDTO.setRows(list);
        return JsonVO.success(pageDTO);
    }

    @Override
    @PutMapping("/{id}")
    @Operation(summary = "修改费用性质",
            description = "根据ID修改费用性质信息。修改后新产生的收费记录将使用更新后的名称/编码，已结算的历史记录不受影响")
    public JsonVO<String> update(
            @PathVariable("id") String id,
            @Valid @RequestBody FeeNatureDTO dto) {
        feeNatureService.updateFeeNature(id, dto);
        return JsonVO.success("修改成功");
    }

    @Override
    @PutMapping("/status")
    @Operation(summary = "停/启用费用性质",
            description = "切换费用性质的启用状态。停用后收费结算下拉框不再显示该项，新收费不可选择；已有结算记录不受影响，仍可正常查询统计")
    public JsonVO<String> updateStatus(
            @RequestParam("id") String id,
            @RequestParam("isEnabled") Integer isEnabled) {
        if (isEnabled != 0 && isEnabled != 1) {
            return JsonVO.fail("状态参数错误，只能为 0（停用）或 1（启用）");
        }
        feeNatureService.updateStatus(id, isEnabled);
        String statusText = isEnabled == 1 ? "启用" : "停用";
        return JsonVO.success(statusText + "成功");
    }

    @Override
    @GetMapping("/nameList")
    @Operation(summary = "获取费用性质名称",
            description = "获取所有已启用的费用性质，供门诊/住院收费结算界面下拉框选择，常用于患者缴费时确定费用类别")
    public JsonVO<List<FeeNatureVO>> getNameList() {
        // Mock 数据：返回启用的费用性质用于下拉框
        List<FeeNatureVO> list = new ArrayList<>();
        FeeNatureVO vo1 = new FeeNatureVO();
        vo1.setId("1");
        vo1.setName("自费");
        vo1.setBusNo("1");
        vo1.setActiveFlag(1);
        vo1.setDisplayOrder(1);
        vo1.setCategoryEnum("自费");
        vo1.setStatusEnum("ACTIVE");
        list.add(vo1);

        FeeNatureVO vo2 = new FeeNatureVO();
        vo2.setId("2");
        vo2.setName("省医保");
        vo2.setBusNo("3");
        vo2.setActiveFlag(1);
        vo2.setDisplayOrder(2);
        vo2.setCategoryEnum("医保");
        vo2.setStatusEnum("ACTIVE");
        list.add(vo2);

        FeeNatureVO vo3 = new FeeNatureVO();
        vo3.setId("3");
        vo3.setName("市医保");
        vo3.setBusNo("2");
        vo3.setActiveFlag(1);
        vo3.setDisplayOrder(3);
        vo3.setCategoryEnum("医保");
        vo3.setStatusEnum("ACTIVE");
        list.add(vo3);

        return JsonVO.success(list);
    }

    @Override
    @PostMapping
    @Operation(summary = "添加费用性质",
            description = "新增一条费用性质记录（如新增医保类型）。添加后即时生效，收费结算模块可立即选用")
    public JsonVO<String> add(@Valid @RequestBody FeeNatureDTO dto) {
        // 校验编码是否已存在（Mock）
        if (dto.getBusNo() == null || dto.getBusNo().isEmpty()) {
            return JsonVO.fail("费用性质编码不能为空");
        }
        // 模拟重复编码校验
        if ("999".equals(dto.getBusNo())) {
            return JsonVO.fail("费用性质编码已存在，请重新输入");
        }
        // TODO: 实际实现后续由开发者补充
        return JsonVO.success("添加成功");
    }

    @Override
    @DeleteMapping("/{id}")
    @Operation(summary = "删除费用性质",
            description = "根据ID删除费用性质。注意：若该费用性质已被收费结算记录引用（存在关联患者缴费数据），则不允许删除，需先停用或处理关联数据")
    public JsonVO<String> delete(@PathVariable("id") String id) {
        // 校验ID是否为空
        if (id == null || id.isEmpty()) {
            return JsonVO.fail("ID参数错误");
        }
        // 模拟删除时检查关联数据（Mock：假设 ID=1 的数据被引用，不能删除）
        if ("1".equals(id)) {
            return JsonVO.fail("该费用性质已被使用，无法删除");
        }
        // TODO: 实际实现后续由开发者补充
        return JsonVO.success("删除成功");
    }
}
