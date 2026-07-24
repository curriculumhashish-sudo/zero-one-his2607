package com.zeroone.star.sysmgr.controller;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j2.sysmgr.rent.RentDTO2;
import com.zeroone.star.project.dto.j2.sysmgr.rentdto.RentAddDTO;
import com.zeroone.star.project.dto.j2.sysmgr.rentdto.RentDTO;
import com.zeroone.star.project.j2.sysmgr.rent.RentApis;
import com.zeroone.star.project.dto.j2.sysmgr.rent.RentNameDTO;
import com.zeroone.star.project.query.j2.sysmgr.rent2.RentNameQuery;
import com.zeroone.star.project.query.j2.sysmgr.rent2.RentQuery;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;
import org.springframework.web.bind.annotation.*;
/**
 * <p>
 * 描述：租户管理控制器
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 鹿
 * @version 1.0.0
 */

@RestController
@Tag(name = "租户管理")
@RequestMapping("/rent")
public class RentController implements RentApis {

    @Operation(summary = "获取租户名称")
    @GetMapping("/query-name")
    @Override
    public JsonVO<PageDTO<RentNameDTO>> queryName(RentNameQuery rentNameQuery) {
        return null;
    }
    @Operation(summary = "获取租户列表（条件+分页）")
    @GetMapping("/query-all")
    @Override
    public JsonVO<PageDTO<RentDTO2>> queryAll(RentQuery rentQuery) {
        return null;
    }

    @Operation(summary = "新增租户")
    @PostMapping("/addRent")
    @Override
    public JsonVO<String> addRent(@RequestBody RentAddDTO addDTO) {
        return null;
    }
    @Operation(summary = "修改租户")
    @PutMapping("/updateRent")
    @Override
    public JsonVO<String> updateRent(@RequestBody RentDTO updateDTO) {
        return null;
    }
    @Operation(summary = "删除租户")
    @DeleteMapping("/deleteRent{id}")
    @Override
    public JsonVO<String> deleteRent(@PathVariable String id) {
        return null;
    }
}
