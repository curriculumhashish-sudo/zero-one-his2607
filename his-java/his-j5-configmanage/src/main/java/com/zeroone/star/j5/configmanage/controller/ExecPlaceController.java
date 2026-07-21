package com.zeroone.star.j5.configmanage.controller;

import com.zeroone.star.j5.configmanage.ExecPlaceApis;
import com.zeroone.star.j5.configmanage.dto.ExecPlaceDTO;
import com.zeroone.star.j5.configmanage.service.IExecPlaceService;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import org.springframework.web.bind.annotation.*;

/**
 * <p>
 * 描述：执行地点管理控制器
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 阿伟学长
 * @version 1.0.0
 */
@RestController
@RequestMapping("/execPlace")
@Tag(name = "执行地点管理", description = "执行地点增删改接口")
public class ExecPlaceController implements ExecPlaceApis {

    @Resource
    private IExecPlaceService execPlaceService;

    @Operation(summary = "新增执行地点")
    @PostMapping("/add")
    @Override
    public JsonVO<String> add(@RequestBody ExecPlaceDTO execPlaceDTO) {
        return execPlaceService.add(execPlaceDTO);
    }

    @Operation(summary = "修改执行地点")
    @PutMapping("/update")
    @Override
    public JsonVO<String> update(@RequestBody ExecPlaceDTO execPlaceDTO) {
        return execPlaceService.update(execPlaceDTO);
    }

    @Operation(summary = "删除执行地点")
    @DeleteMapping("/delete/{id}")
    @Override
    public JsonVO<String> delete(@PathVariable("id") Long id) {
        return execPlaceService.delete(id);
    }
}
