package com.zeroone.star.sample.controller;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.sample.SampleAddDTO;
import com.zeroone.star.project.dto.sample.SampleDTO;
import com.zeroone.star.project.query.sample.SampleQuery;
import com.zeroone.star.project.sample.SampleApis;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.sample.entity.Sample;
import com.zeroone.star.sample.service.ISampleService;
import com.zeroone.star.sample.service.impl.MsSampleMapper;
import io.swagger.v3.core.util.Json;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.enums.ParameterIn;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.*;

import java.util.List;

/**
 * <p>
 * 演示示例表 前端控制器
 * </p>
 *
 * @author 告别
 * @since 2026-07-12
 */
@RestController
@Tag(name="sample",description = "Sample接口")
@Validated
public class SampleController implements SampleApis {

    @Resource
    ISampleService iSampleService;

    @Resource
    MsSampleMapper msSampleMapper;

    @PostMapping("/add-sample")
    @Operation(summary = "添加示例")
    @Override
    public JsonVO<String> addSample(@Validated @RequestBody SampleAddDTO addDto) {
        Sample sample = msSampleMapper.sampleAddDtoToSample(addDto);
        if(iSampleService.save(sample)){
            return JsonVO.success(sample.getId());
        }
        return JsonVO.fail(null);
    }

    @PutMapping("/modify-sample")
    @Operation(summary = "修改示例")
    @Override
    public JsonVO<String> modifySample(@Validated @RequestBody SampleDTO dto) {
        Sample sample = msSampleMapper.sampleDtoToSample(dto);
        if(iSampleService.updateById(sample)){
            return JsonVO.success(dto.getId());
        }
        return JsonVO.fail(null);
    }

    @GetMapping("/query-all")
    @Operation(summary = "分页查询所有示例")
    @Override
    public JsonVO<PageDTO<SampleDTO>> queryAll(@Validated SampleQuery condition) {
        PageDTO<SampleDTO> listAll = iSampleService.listAll(condition);
        return JsonVO.success(listAll);
    }

    @GetMapping("/query-one")
    @Operation(summary = "根据编号查询")
    @Parameter(name = "id",description = "编号",required = true,in = ParameterIn.QUERY,example = "1d7b0520e93e72715d5d6af1fb7d9a37")
    @Override
    public JsonVO<SampleDTO> queryById(String id) {
        return JsonVO.success(iSampleService.getById(id));
    }

    @DeleteMapping("/remove-sample")
    @Operation(summary = "删除实例")
    @Override
    public JsonVO<List<String>> removeSample(List<String> ids) {
        if(iSampleService.removeByIds(ids)){
            return JsonVO.success(ids);
        }
        return JsonVO.fail(null);
    }
}
