package com.zeroone.star.sysmgr.controller;

import com.zeroone.star.project.dto.j2.sysmgr.DictItemUpdateDTO;
import com.zeroone.star.project.j2.sysmgr.DictItemApis;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import org.springframework.web.bind.annotation.*;

@RestController
@Tag(name = "字典管理器")
public class DictItemController implements DictItemApis {

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
