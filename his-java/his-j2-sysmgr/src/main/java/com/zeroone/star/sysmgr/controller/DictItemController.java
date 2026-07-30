package com.zeroone.star.sysmgr.controller;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j2.basedata.DictItemUpdateDTO;
import com.zeroone.star.project.dto.j2.sysmgr.dictiemdto.DictItemAddDTO;
import com.zeroone.star.project.dto.j2.sysmgr.dictiemdto.DictItemDTO;
import com.zeroone.star.project.j2.sysmgr.DictItemApis;
import com.zeroone.star.project.query.j2.sysmgr.DictItemQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.sysmgr.service.ITerConceptService;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;

import java.util.List;

/**
 * 字典项控制器
 */
@RestController
@Tag(name = "字典项管理")
public class DictItemController implements DictItemApis {

    @Autowired
    private ITerConceptService terConceptService;

    @Override
    public JsonVO<List<String>> queryDictNameList(DictItemQuery query) {
        return null;
    }

    @Override
    public JsonVO<PageDTO<DictItemDTO>> queryAll(DictItemQuery query) {
        return null;
    }

    @Override
    public JsonVO<String> addDictItem(DictItemAddDTO addDTO) {
        return null;
    }

    @Operation(summary = "修改字典项")
    @PutMapping("/dictItem")
    @Override
    public JsonVO<String> updateDict(@RequestBody DictItemUpdateDTO updateDTO) {
        terConceptService.updateDict(updateDTO);
        return JsonVO.success("修改成功");
    }

    @Operation(summary = "删除字典项")
    @DeleteMapping("/dictItem/{id}")
    @Override
    public JsonVO<String> deleteDict(@PathVariable("id") Long id) {
        terConceptService.deleteDict(id);
        return JsonVO.success("删除成功");
    }
}
