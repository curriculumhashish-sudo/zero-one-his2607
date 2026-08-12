package com.zeroone.star.basedata.controller;


import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.alibaba.nacos.common.utils.StringUtils;
import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.zeroone.star.basedata.entity.Dict;
import com.zeroone.star.basedata.mapper.DictMapper;
import com.zeroone.star.basedata.service.ITerConceptService;
import com.zeroone.star.project.dto.PageDTO;

import com.zeroone.star.project.dto.j2.basedata.dictionarydto.DictItemUpdateDTO;
import com.zeroone.star.project.dto.j2.sysmgr.dictiemdto.DictItemAddDTO;
import com.zeroone.star.project.dto.j2.sysmgr.dictiemdto.DictItemDTO;
import com.zeroone.star.project.j2.basedata.dictionary.DictItemApis;
import com.zeroone.star.project.query.j2.sysmgr.DictItemQuery;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;

import lombok.RequiredArgsConstructor;
import org.springframework.beans.factory.annotation.Autowired;
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
@RequestMapping("/dictionary")
@Tag(name = "字典管理")
@RequiredArgsConstructor
public class DictItemController implements DictItemApis {

    @Autowired
    private ITerConceptService terConceptService;

    private final DictMapper dictMapper;
    @Operation(summary = "获取字典名称列表")
    @GetMapping("/queryDictNameList")
    @Override
    public JsonVO<List<String>> queryDictNameList(DictItemQuery query) {
        List<String> names = dictMapper.selectDictNameList();
        return JsonVO.success(names);
    }

    @Operation(summary = "获取字典列表(条件+分页)")
    @GetMapping("/queryDictItem")
    @Override
    public JsonVO<PageDTO<DictItemDTO>> queryAllDictItems(DictItemQuery query) {
        Page<Dict> page = new Page<>(query.getPageIndex(), query.getPageSize());
        LambdaQueryWrapper<Dict> wrapper = new LambdaQueryWrapper<>();
        wrapper.eq(query.getCodeSystemId() != null, Dict::getCodeSystemId, query.getCodeSystemId());
        wrapper.eq(StringUtils.hasText(query.getDictCode()), Dict::getCode, query.getDictCode());
        if (StringUtils.hasText(query.getDicitemKeyword())) {
            wrapper.and(w -> w
                    .like(Dict::getDisplay, query.getDicitemKeyword())
                    .or()
                    .like(Dict::getCode, query.getDicitemKeyword()));
        }
        wrapper.orderByAsc(Dict::getSortOrder);  // ✅ sortOrder
        Page<Dict> result = dictMapper.selectPage(page, wrapper);
        PageDTO<DictItemDTO> pageDTO = PageDTO.create(result, dict -> {
            DictItemDTO dto = new DictItemDTO();
            dto.setDicId(String.valueOf(dict.getId()));
            dto.setCodeSystemId(dict.getCodeSystemId());
            dto.setDicCode(dict.getCode());
            dto.setDicName(dict.getDisplay());
            dto.setDicDescription(dict.getDefinition());
            dto.setDicSort(dict.getSortOrder() != null ? dict.getSortOrder().intValue() : null);     // ✅ sortOrder
            return dto;
        });
        return JsonVO.success(pageDTO);
    }

    @Operation(summary = "新增字典")
    @PostMapping("/addDictItem")
    @Override
    public JsonVO<String> addDictItem(@RequestBody DictItemAddDTO addDTO) {
        if (addDTO.getCodeSystemId() == null) {
            return JsonVO.fail("codeSystemId不能为空");
        }
        if (!StringUtils.hasText(addDTO.getDicCode())) {
            return JsonVO.fail("字典编码(dicCode)不能为空");
        }
        if (!StringUtils.hasText(addDTO.getDicName())) {
            return JsonVO.fail("字典名称(dicName)不能为空");
        }
        Dict dict = new Dict();
        dict.setCodeSystemId(addDTO.getCodeSystemId());
        dict.setCode(addDTO.getDicCode());              // ✅ code
        dict.setDisplay(addDTO.getDicName());           // ✅ display
        dict.setDefinition(addDTO.getDicDescription()); // ✅ definition
        dict.setSortOrder(addDTO.getDicSort() != null ? addDTO.getDicSort().longValue() : null);         // ✅ sortOrder
        int rows = dictMapper.insert(dict);
        return rows > 0 ? JsonVO.success("新增成功") : JsonVO.fail("新增失败");
    }
    @Operation(summary = "修改字典")
    @PutMapping("/dictItem")
    @Override
    public JsonVO<String> updateDict(@RequestBody DictItemUpdateDTO updateDTO) {
        terConceptService.updateDict(updateDTO);
        return JsonVO.success("修改成功");
    }

    @Operation(summary = "删除字典")
    @DeleteMapping("/dictItem/{id}")
    @Override
    public JsonVO<String> deleteDict(@PathVariable("id") Long id) {
        terConceptService.deleteDict(id);
        return JsonVO.success("删除成功");
    }

}
