package com.zeroone.star.j3.fee.controller;

import com.zeroone.star.project.dto.j3.fee.FeeNatureDTO;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.j3.fee.FeeNatureApis;
import com.zeroone.star.project.query.j3.fee.FeeNatureQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j3.fee.FeeNatureVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.validation.Valid;
import org.springframework.web.bind.annotation.*;

import java.util.ArrayList;
import java.util.List;

/**
 * <p>
 * 描述：费用性质管理控制器（文档桩实现，仅用于生成前后端交互文档）
 * </p>
 * @author 白文儿
 * @version 1.0.0
 */
@RestController
@RequestMapping("/fee")
@Tag(name = "费用性质管理", description = "费用性质相关操作接口")
public class FeeNatureController implements FeeNatureApis {

    @Override
    @GetMapping("/page")
    @Operation(summary = "获取费用性质列表（条件+分页）", description = "支持按名称或编码模糊搜索，返回分页结果")
    public JsonVO<PageDTO<FeeNatureVO>> getPageList(@Valid FeeNatureQuery query) {
        // Mock 数据：原型图中的示例数据
        List<FeeNatureVO> mockList = new ArrayList<>();
        FeeNatureVO vo1 = new FeeNatureVO();
        vo1.setId(1L);
        vo1.setName("自费");
        vo1.setCode("1");
        vo1.setIsEnabled(1);
        mockList.add(vo1);

        FeeNatureVO vo2 = new FeeNatureVO();
        vo2.setId(2L);
        vo2.setName("省医保");
        vo2.setCode("3");
        vo2.setIsEnabled(1);
        mockList.add(vo2);

        PageDTO<FeeNatureVO> pageDTO = new PageDTO<>();
        pageDTO.setPageIndex(query.getPageIndex());
        pageDTO.setPageSize(query.getPageSize());
        pageDTO.setTotal(2L);
        pageDTO.setPages(1L);
        pageDTO.setRows(mockList);

        return JsonVO.success(pageDTO);
    }

    @Override
    @PutMapping("/{id}")
    @Operation(summary = "修改费用性质", description = "根据ID修改费用性质名称和编码")
    public JsonVO<String> update(
            @PathVariable("id") Long id,
            @Valid @RequestBody FeeNatureDTO dto) {
        // TODO: 实际实现后续由开发者补充
        return JsonVO.success("修改成功");
    }

    @Override
    @PutMapping("/status")
    @Operation(summary = "停/启用费用性质", description = "切换费用性质的启用状态")
    public JsonVO<String> updateStatus(
            @RequestParam("id") Long id,
            @RequestParam("isEnabled") Integer isEnabled) {
        // 校验状态参数
        if (isEnabled != 0 && isEnabled != 1) {
            return JsonVO.fail("状态参数错误，只能为 0（停用）或 1（启用）");
        }
        // TODO: 实际实现后续由开发者补充
        String statusText = isEnabled == 1 ? "启用" : "停用";
        return JsonVO.success(statusText + "成功");
    }

    @Override
    @GetMapping("/nameList")
    @Operation(summary = "获取费用性质名称列表", description = "获取所有启用的费用性质名称，用于下拉框选择")
    public JsonVO<List<FeeNatureVO>> getNameList() {
        // 实际查询：SELECT id, name FROM t_fee_nature WHERE is_enabled = 1 ORDER BY sort_order
        // TODO: 实际实现后续由开发者补充
        List<FeeNatureVO> list = null;
        return JsonVO.success(list);
    }

    @Override
    @PostMapping
    @Operation(summary = "添加费用性质", description = "新增一条费用性质记录")
    public JsonVO<String> add(@Valid @RequestBody FeeNatureDTO dto) {
        // 校验编码是否已存在（Mock）
        if (dto.getCode() == null || dto.getCode().isEmpty()) {
            return JsonVO.fail("费用性质编码不能为空");
        }
        // 模拟重复编码校验
        if ("999".equals(dto.getCode())) {
            return JsonVO.fail("费用性质编码已存在，请重新输入");
        }
        // TODO: 实际实现后续由开发者补充
        return JsonVO.success("添加成功");
    }

    @Override
    @DeleteMapping("/{id}")
    @Operation(summary = "删除费用性质", description = "根据ID删除费用性质记录")
    public JsonVO<String> delete(@PathVariable("id") Long id) {
        // 校验ID是否存在（Mock）
        if (id == null || id <= 0) {
            return JsonVO.fail("ID参数错误");
        }
        // 模拟删除时检查关联数据（Mock：假设 ID=1 的数据被引用，不能删除）
        if (id == 1L) {
            return JsonVO.fail("该费用性质已被使用，无法删除");
        }
        // TODO: 实际实现后续由开发者补充
        return JsonVO.success("删除成功");
    }
}
