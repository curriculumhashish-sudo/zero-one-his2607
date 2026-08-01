package com.zeroone.star.outpatient.controller;

import com.zeroone.star.outpatient.service.MinXiService;
import com.zeroone.star.project.j8.mzfymx.MinXiApis;
import com.zeroone.star.project.query.j8.mzfymx.MinXiExportQuery;
import com.zeroone.star.project.query.j8.mzfymx.MinXiQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j8.mzfymx.ExportFileVO;
import com.zeroone.star.project.vo.j8.mzfymx.MinXiPageVO;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import org.springframework.validation.annotation.Validated;
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
@RestController
@Validated
@Tag(name = "门诊费用明细")
public class mzfymxController implements MinXiApis {

    @Resource
    private MinXiService minXiService;

    /**
     * 接口 1：门诊费用明细-分页查询（返回 rows + 全量 8 个金额合计）
     */
    @Override
    public JsonVO<MinXiPageVO> getMinXiList(MinXiQuery query) {
        return JsonVO.success(minXiService.queryPage(query));
    }

    /**
     * 接口 2：门诊费用明细-导出 Excel（复用 queryPage 查询逻辑，去掉 LIMIT 分页）
     */
    @Override
    public JsonVO<ExportFileVO> exportMinXi(MinXiExportQuery query) {
        return JsonVO.success(minXiService.export(query));
    }

}