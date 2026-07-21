package com.zeroone.star.sysmgr.controller;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j2.sysmgr.rentdto.RentAddDTO;
import com.zeroone.star.project.dto.j2.sysmgr.rentdto.RentDTO;
import com.zeroone.star.project.j2.sysmgr.RentApis;
import com.zeroone.star.project.query.j2.sysmgr.rent.RentQuery;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import org.springframework.web.bind.annotation.*;

/**
 * <p>
 * 描述：租户管理控制器
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 阿伟学长
 * @version 1.0.0
 */
@RestController
@Tag(name = "租户管理")
public class RentController implements RentApis {
    @Operation(summary = "查询所有租户信息")
    @GetMapping("/queryAll")
    @Override
    public JsonVO<PageDTO<RentDTO>> queryAll(RentQuery query) {
        return null;
    }
    @Operation(summary = "添加租户信息")
    @PostMapping("/addRent")
    @Override
    public JsonVO<String> addRent(@RequestBody RentAddDTO addDTO) {
        return null;
    }
    @Operation(summary = "更新租户信息")
    @PutMapping("/updateRent")
    @Override
    public JsonVO<String> updateRent(@RequestBody RentDTO updateDTO) {
        return null;
    }
    @Operation(summary = "删除租户信息")
    @DeleteMapping("/deleteRent{id}")
    @Override
    public JsonVO<String> deleteRent(@PathVariable String id) {
        return null;
    }
}
