package com.zeroone.star.outpatient.controller;

import com.zeroone.star.project.dto.j8.mzfymx.MinXiDTO;
import com.zeroone.star.project.j8.mzfymx.MinXiApis;
import com.zeroone.star.project.query.j8.mzfymx.MinXiExportQuery;
import com.zeroone.star.project.query.j8.mzfymx.MinXiQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j8.mzfymx.ExportFileVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import org.springframework.web.bind.annotation.*;

/**
 *<p>
 * 描述：门诊费用明细控制器
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 阿伟学长
 * @version 1.0.0
 */
@RequestMapping("/j8/mzfymx")
@RestController
@Tag(name = "门诊费用明细")
public class mzfymxController implements MinXiApis {

    @Operation(summary = "获取门诊费用明细列表")
    @GetMapping("/getMinXiList")
    @Override
    public JsonVO<MinXiDTO> getMinXiList(MinXiQuery query) {
        return null;
    }

    @Operation(summary ="导出门诊费用明细" )
    @PostMapping("/exportMinXi")
    @Override
    public JsonVO<ExportFileVO> exportMinXi(@RequestBody MinXiExportQuery query){
        return null;
    }

}
