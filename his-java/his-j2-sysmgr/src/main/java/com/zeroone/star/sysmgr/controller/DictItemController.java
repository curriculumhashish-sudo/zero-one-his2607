package com.zeroone.star.sysmgr.controller;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j2.sysmgr.dictiemdto.DictItemAddDTO;
import com.zeroone.star.project.dto.j2.sysmgr.dictiemdto.DictItemDTO;
import com.zeroone.star.project.j2.sysmgr.DictItemApis;
import com.zeroone.star.project.query.j2.sysmgr.DictItemQuery;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import org.springframework.web.bind.annotation.*;

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
    @Operation(summary = "查询所有字典信息")
    @GetMapping("/queryDictItem")
    @Override
    public JsonVO<PageDTO<DictItemDTO>> queryAll(DictItemQuery query) {
        return null;
    }
    @Operation(summary = "添加字典信息")
    @PostMapping("/addDictItem")
    @Override
    public JsonVO<String> addDictItem(@RequestBody DictItemAddDTO addDTO) {
        return null;
    }
    @Operation(summary = "更新字典信息")
    @PutMapping("/updateDictItem")
    @Override
    public JsonVO<String> updateDictItem(@RequestBody DictItemDTO updateDTO) {
        return null;
    }
    @Operation(summary = "删除字典信息")
    @DeleteMapping("/deleteDictItem{id}")
    @Override
    public JsonVO<String> deleteDictItem(@PathVariable String id) {
        return null;
    }
}
