package com.zeroone.star.sysmgr.controller;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j2.sysmgr.rentdto.RentAddDTO;
import com.zeroone.star.project.dto.j2.sysmgr.rentdto.RentDTO;
import com.zeroone.star.project.dto.j2.sysmgr.rentdto.RentDTO2;
import com.zeroone.star.project.dto.j2.sysmgr.rentdto.RentNameDTO;
import com.zeroone.star.project.j2.sysmgr.rent.RentApis;
import com.zeroone.star.project.query.j2.sysmgr.rent2.RentNameQuery;
import com.zeroone.star.project.query.j2.sysmgr.rent2.RentQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.sysmgr.service.IWhaleTenantsService;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import lombok.extern.slf4j.Slf4j;
import org.springframework.validation.annotation.Validated;
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
@Slf4j
@RestController
@Tag(name = "租户管理")
@RequestMapping("/rent")
public class RentController implements RentApis {

    @Resource
    private IWhaleTenantsService whaleTenantsService;

    @Operation(summary = "获取租户名称")
    @GetMapping("/query-name")
    @Override
    public JsonVO<PageDTO<RentNameDTO>> queryName(@Validated RentNameQuery rentNameQuery) {
        try {
            PageDTO<RentNameDTO> result = whaleTenantsService.queryTenantNames(rentNameQuery);
            return JsonVO.success(result);
        } catch (Exception e) {
            log.error("获取租户名称异常", e);
            return JsonVO.fail("获取租户名称失败，请稍后重试");
        }
    }

    @Operation(summary = "获取租户列表（条件+分页）")
    @GetMapping("/query-all")
    @Override
    public JsonVO<PageDTO<RentDTO2>> queryAll(@Validated RentQuery rentQuery) {
        try {
            PageDTO<RentDTO2> result = whaleTenantsService.queryTenantList(rentQuery);
            return JsonVO.success(result);
        } catch (Exception e) {
            log.error("获取租户列表异常, code={}, keyword={}", rentQuery.getCode(), rentQuery.getRentKeywords(), e);
            return JsonVO.fail("获取租户列表失败，请稍后重试");
        }
    }

    @Operation(summary = "新增租户")
    @PostMapping("/addRent")
    @Override
    public JsonVO<String> addRent(@RequestBody @Validated RentAddDTO addDTO) {
        try {
            whaleTenantsService.addRent(addDTO);
            return JsonVO.success("新增租户成功");
        } catch (Exception e) {
            log.error("新增租户异常", e);
            return JsonVO.fail("新增租户失败，请稍后重试");
        }
    }

    @Operation(summary = "修改租户")
    @PutMapping("/updateRent/{id}")
    @Override
    public JsonVO<String> updateRent(
            @PathVariable("id") String id,
            @RequestBody @Validated RentDTO updateDTO) {
        try {
            // 将路径中的 id 设置到 DTO 中，确保一致性
            updateDTO.setId(id);
            whaleTenantsService.updateRent(updateDTO);
            return JsonVO.success("修改租户成功");
        } catch (Exception e) {
            log.error("修改租户异常", e);
            return JsonVO.fail("修改租户失败，请稍后重试");
        }
    }

    @Operation(summary = "删除租户")
    @DeleteMapping("/deleteRent/{id}")
    @Override
    public JsonVO<String> deleteRent(@PathVariable String id) {
        try {
            whaleTenantsService.deleteRent(id);
            return JsonVO.success("删除租户成功");
        } catch (Exception e) {
            log.error("删除租户异常，租户id={}", id, e);
            return JsonVO.fail("删除租户失败，请稍后重试");
        }
    }
}