package com.zeroone.star.outpatient.service;

import com.zeroone.star.project.query.j8.mzfymx.MinXiExportQuery;
import com.zeroone.star.project.query.j8.mzfymx.MinXiQuery;
import com.zeroone.star.project.vo.j8.mzfymx.ExportFileVO;
import com.zeroone.star.project.vo.j8.mzfymx.MinXiPageVO;

/**
 * <p>
 * 描述：门诊费用明细（mzfymx）Service 接口（只定义业务方法，不含实现）
 *      实现类：{@link com.zeroone.star.outpatient.service.impl.MinXiServiceImpl}
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 阿伟学长
 * @version 1.0.0
 */
public interface MinXiService {

    /**
     * 接口 1：分页查询门诊费用明细（方案 B 纯平铺，1 行 = 1 条收费项，同一次就诊的就诊字段每行重复）
     *        返回 MinXiPageVO = rows + totalRowCount + pageNo/pageSize(新增返回) + 8 个 *Sum(全量合计)
     * @param query 查询条件（含分页参数 pageIndex/pageSize）
     * @return 纯平铺分页结果 + 全量金额合计
     */
    MinXiPageVO queryPage(MinXiQuery query);

    /**
     * 接口 2：按查询条件生成 Excel 报表，返回 Base64 文件内容 / FastDFS 下载 URL 供浏览器自动下载
     *        （查询条件 = queryPage 接口去掉分页 2 个参数，剩下 9 个筛选条件完全一致）
     * @param query 查询条件（不含分页参数）
     * @return Excel 文件导出对象（文件名、内容类型、Base64/URL）
     */
    ExportFileVO export(MinXiExportQuery query);
}