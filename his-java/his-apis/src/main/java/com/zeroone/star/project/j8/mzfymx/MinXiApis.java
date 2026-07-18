package com.zeroone.star.project.j8.mzfymx;

import com.zeroone.star.project.dto.j8.mzfymx.MinXiDTO;
import com.zeroone.star.project.query.j8.mzfymx.MinXiExportQuery;
import com.zeroone.star.project.query.j8.mzfymx.MinXiQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j8.mzfymx.ExportFileVO;

/**
 * <p>
 * 描述：门诊费用明细接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 阿伟学长
 * @version 1.0.0
 */
public interface MinXiApis {
    /**
     * 获取门诊费用明细列表（ 条件 + 分页 ）
     * @param query 查询对象
     * @return 分页结果
     */
    JsonVO<MinXiDTO> getMinXiList(MinXiQuery query);

    /**
     * 导出门诊费用明细报表（不分页，全量数据）
     * @param query 查询对象（不含分页参数）
     * @return Excel文件导出对象（包含文件名、内容类型、Base64编码的文件内容）
     */
    JsonVO<ExportFileVO> exportMinXi(MinXiExportQuery query);

}