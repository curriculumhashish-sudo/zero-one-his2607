package com.zeroone.star.project.j8.mzfymx;

import com.zeroone.star.project.query.j8.mzfymx.MinXiExportQuery;
import com.zeroone.star.project.query.j8.mzfymx.MinXiQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j8.mzfymx.ExportFileVO;
import com.zeroone.star.project.vo.j8.mzfymx.MinXiPageVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.validation.Valid;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.*;

/**
 * <p>
 * 描述：门诊费用明细接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 阿伟学长
 * @version 1.0.0
 */
@RequestMapping("/j8/mzfymx")
@Tag(name = "门诊费用明细")
public interface MinXiApis {
    /**
     * 获取门诊费用明细列表（条件 + 分页，方案 B 纯平铺：1 行 = 1 条收费项）
     * ⚠️ 返回结构 MinXiPageVO：
     *   - rows[]          明细行列表：每个元素 = 1 条收费项
     *                     同一次就诊多条收费项 → 门诊号/患者/挂号科室/就诊日期 4 列完全相同（重复展示）
     *   - totalRowCount   满足条件的明细总行数（Long 类型，全量 SUM，不带 LIMIT，用于前端分页栏显示）
     *   - pageNo          ⚠️ 新增返回：当前页码（= 入参 pageIndex 原值，不动 PageQuery 父类，仅前端分页栏高亮当前页用）
     *   - pageSize        ⚠️ 新增返回：每页大小（= 入参 pageSize 原值，仅前端分页栏下拉高亮用）
     *   - 8 个 *Sum       全量金额合计（SUM 所有满足条件的 charge_item，不带 LIMIT，对应原型图顶部红色数字）
     * @param query 查询对象（含分页参数 pageIndex/pageSize）
     * @return 纯平铺分页结果 + 全量合计
     */
    @Operation(summary = "获取门诊费用明细列表")
    @GetMapping("/getMinXiList")
    JsonVO<MinXiPageVO> getMinXiList(@Validated MinXiQuery query);

    /**
     * 导出门诊费用明细报表（不分页，全量数据导出 xlsx）
     * ⚠️ 与查询接口 getMinXiList 的关系：
     *   - 相同：所有筛选条件（dateFrom/dateTo/deptId/feeType/feeTypeDetail/patientKeyword/projectName/internalCode/tradeDirection）完全一致
     *   - 不同：不含 pageIndex/pageSize 分页参数，导出满足条件的**全量数据**
     *     （备注：导出接口就比查询明细接口少了分页参数而已）
     *   - 导出结构：每行 = 1 条 MinXiExportDTO（同查询 rows[] 结构，就诊字段每行重复），最后追加合计行
     * @param query 查询对象（不含分页参数）
     * @return Excel 文件导出对象（包含文件名、内容类型、Base64 编码的 xlsx 文件内容）
     */
    @Operation(summary ="导出门诊费用明细")
    @PostMapping("/exportMinXi")
    JsonVO<ExportFileVO> exportMinXi(@RequestBody @Valid MinXiExportQuery query);

}