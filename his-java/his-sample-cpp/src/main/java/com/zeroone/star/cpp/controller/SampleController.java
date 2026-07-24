package com.zeroone.star.cpp.controller;

import cn.hutool.core.bean.BeanUtil;
import com.zeroone.star.cpp.service.ISampleService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.sample.SampleAddDTO;
import com.zeroone.star.project.dto.sample.SampleDTO;
import com.zeroone.star.project.query.sample.SampleQuery;
import com.zeroone.star.project.sample.SampleApis;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import org.springframework.web.bind.annotation.*;

import java.util.List;

/**
 * <p>
 * 描述：测试调用声明式服务
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 阿伟学长
 * @version 1.0.0
 */
@RestController
@Tag(name = "CPP服务调用")
public class SampleController implements SampleApis {
    @Resource
    ISampleService service;

    @Operation(summary = "添加数据")
    @PostMapping("/add-sample")
    @Override
    public JsonVO<String> addSample(@RequestBody SampleAddDTO addDto) {
        return service.post(addDto);
    }

    @Operation(summary = "修改数据")
    @PutMapping("/modify-sample")
    @Override
    public JsonVO<String> modifySample(@RequestBody SampleDTO dto) {
        return service.put(dto);
    }

    @Operation(summary = "分页查询")
    @GetMapping("/query-all")
    @Override
    public JsonVO<PageDTO<SampleDTO>> queryAll(SampleQuery condition) {
        return service.get(BeanUtil.beanToMap(condition, false, true));
    }

    @Override
    public JsonVO<SampleDTO> queryById(String id) {
        return null;
    }

    @Operation(summary = "删除数据")
    @DeleteMapping("/remove-sample")
    @Override
    public JsonVO<List<String>> removeSample(@RequestBody List<String> ids) {
        return service.delete(ids);
    }
}
