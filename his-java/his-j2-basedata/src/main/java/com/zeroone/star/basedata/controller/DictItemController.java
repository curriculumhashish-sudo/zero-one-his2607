package com.zeroone.star.basedata.controller;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j2.basedata.DictItemUpdateDTO;
import com.zeroone.star.project.dto.j2.sysmgr.dictiemdto.DictItemAddDTO;
import com.zeroone.star.project.dto.j2.sysmgr.dictiemdto.DictItemDTO;
import com.zeroone.star.project.j2.sysmgr.DictItemApis;
import com.zeroone.star.project.query.j2.sysmgr.DictItemQuery;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import org.springframework.web.bind.annotation.*;

import java.util.List;

/**
 * <p>
 * 描述：字典管理控制器
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 阿伟学长
 * @version 1.0.0
 */
@RestController
@Tag(name = "字典管理")
public class DictItemController implements DictItemApis {

    @Operation(summary = "获取字典名称列表")
    @GetMapping("/queryDictNameList")
    @Override
    public JsonVO<List<String>> queryDictNameList() {
        return null;
    }

    @Operation(summary = "获取字典列表(条件+分页)",description = "获取字典列表(条件+分页)")
    @GetMapping("/queryDictItem")
    @Override
    public JsonVO<PageDTO<DictItemDTO>> queryAll(DictItemQuery query) {
        return null;
    }
    @Operation(summary = "新增字典")
    @PostMapping("/addDictItem")
    @Override
    public JsonVO<String> addDictItem(@RequestBody DictItemAddDTO addDTO) {
        return null;
    }
    @Operation(summary = "修改字典")
    @PutMapping("/dictItem")
    @Override
    public JsonVO<String> updateDict(@RequestBody DictItemUpdateDTO updateDTO) {
        return null;
    }

    @Operation(summary = "删除字典")
    @DeleteMapping("/dictItem/{id}")
    @Override
    public JsonVO<String> deleteDict(@PathVariable("id") Long id) {
        return null;
    }

}
