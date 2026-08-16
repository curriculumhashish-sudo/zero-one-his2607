package com.zeroone.star.project.j6.report;

import com.zeroone.star.project.query.j6.report.FeeDetailExportQuery;
import com.zeroone.star.project.query.j6.report.FeeDetailQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j6.report.OutpatientFeeDetailsVo;
import jakarta.servlet.http.HttpServletResponse;

public interface OutpatientFeeDetailsApis {
    /**
     * 门诊费用明细列表（条件+分页）
     * @param query 查询条件（含分页参数）
     * @return 分页费用明细数据
     */
    JsonVO<OutpatientFeeDetailsVo> queryFeeDetail(FeeDetailQuery query);

    /**
     * 门诊费用明细报表导出（文件流）
     * @param query 查询条件
     * @param response HttpServletResponse
     */
    void exportFeeDetail(FeeDetailExportQuery query, HttpServletResponse response);

}
