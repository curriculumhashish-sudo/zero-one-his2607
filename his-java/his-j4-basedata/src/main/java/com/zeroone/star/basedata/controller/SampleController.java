package com.zeroone.star.basedata.controller;

import com.zeroone.star.basedata.service.ISampleService;
import com.zeroone.star.project.dto.j4.sample.SpecimenDefinitionDeleteDTO;
import com.zeroone.star.project.dto.j4.sample.SpecimenDefinitionStatusDTO;
import com.zeroone.star.project.dto.j4.sample.SpecimenDefinitionUpdateDTO;
import com.zeroone.star.project.j4.basedata.SampleApis;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j4.sample.SpecimenDefinitionOperateVO;
import com.zeroone.star.project.vo.j4.sample.SpecimenOptionVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import jakarta.validation.Valid;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j4.sample.SpecimenDefinitionAddDTO;
import com.zeroone.star.project.query.j4.sample.SpecimenDefinitionQuery;
import com.zeroone.star.project.vo.j4.sample.SpecimenDefinitionDetailVO;
import org.springframework.web.bind.annotation.*;

import java.util.List;

/**
 * <p>
 * 描述：样本管理控制器
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@RestController
@RequestMapping("/sample")
@Tag(name = "样本管理", description = "样本管理接口")
public class SampleController implements SampleApis {

    @Resource
    private ISampleService sampleService;

    @Operation(summary = "启用/停用样本定义", description = "根据主键与乐观锁版本号，更新样本定义的发布状态")
    @PutMapping("/update-sample-status")
    @Override
    public JsonVO<SpecimenDefinitionOperateVO> updateSampleStatus(@Valid @RequestBody SpecimenDefinitionStatusDTO dto) {
        return JsonVO.success(sampleService.updateSampleStatus(dto));
    }

    @Operation(summary = "删除样本定义", description = "根据主键与乐观锁版本号，软删除样本定义")
    @DeleteMapping("/delete-sample")
    @Override
    public JsonVO<SpecimenDefinitionOperateVO> deleteSample(@Valid @RequestBody SpecimenDefinitionDeleteDTO dto) {
        return JsonVO.success(sampleService.deleteSample(dto));
    }

    @Operation(summary = "修改样本定义", description = "根据主键与乐观锁版本号，修改样本名称、标本种类")
    @PutMapping("/update-sample")
    @Override
    public JsonVO<SpecimenDefinitionOperateVO> updateSample(@Valid @RequestBody SpecimenDefinitionUpdateDTO dto) {
        return JsonVO.success(sampleService.updateSample(dto));
    }

    /**
     * 获取样本名称下拉列表（当前租户下所有样本）
     * 供其他组模块调用，前端渲染成下拉选项，选择后存储样本ID
     */
    @Operation(summary = "获取样本名称", description = "获取当前租户下所有样本定义的名称下拉选项，供其他模块选择并存储样本ID")
    @GetMapping("list-sample-names")
    @Override
    public JsonVO<List<SpecimenOptionVO>> listSampleNames() {
        return JsonVO.success(sampleService.listSampleNames());
    }

    /**
     * 获取样本列表（条件+分页）
     */
    @Operation(summary = "获取样本列表", description = "支持按名称/业务编码/拼音/五笔模糊查询，按标本种类/状态精确筛选，返回分页结果")
    @GetMapping("query-sample-list")
    @Override
    public JsonVO<PageDTO<SpecimenDefinitionDetailVO>> querySampleList(@Valid @RequestBody SpecimenDefinitionQuery query) {
        return JsonVO.success(sampleService.querySamplePage(query));
    }

    /**
     * 新增样本定义
     */
    @Operation(summary = "新增样本定义", description = "根据传入的样本数据新增一条样本定义记录，发布状态默认置为启用")
    @PostMapping("add-sample")
    @Override
    public JsonVO<String> addSample(@Valid @RequestBody SpecimenDefinitionAddDTO dto) {
        return JsonVO.success(sampleService.addSample(dto));
    }
}
