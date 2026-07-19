package com.zeroone.star.sysmgr.controller;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j2.sys.RentDTO;
import com.zeroone.star.project.dto.j2.sys.RentNameDTO;
import com.zeroone.star.project.j2.sys.RentApis;
import com.zeroone.star.project.query.j2.sys.RentNameQuery;
import com.zeroone.star.project.query.j2.sys.RentQuery;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;

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
@Tag(name = "租户管理控制器")
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
    public JsonVO<PageDTO<RentDTO>> queryAll(RentQuery rentQuery) {
        return null;
    }
}
