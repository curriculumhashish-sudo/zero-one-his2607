package com.zeroone.star.project.j6.report;

import com.zeroone.star.project.query.j6.report.FeeDetailQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j6.report.ExportResultVo;
import com.zeroone.star.project.vo.j6.report.OutpatientFeeDetailsVo;

public interface OutpatientFeeDetailsApis {
    /**
     * 门诊费用明细列表（条件+分页）
     * @param query 查询条件（含分页参数）
     * @return 分页费用明细数据
     */
    JsonVO<OutpatientFeeDetailsVo> queryFeeDetail(FeeDetailQuery query);

    /**
     * 门诊费用明细报表导出
     * @param query 查询条件（与列表查询条件一致）
     * @return 导出文件下载URL
     */
    JsonVO<ExportResultVo> exportFeeDetail(FeeDetailQuery query);

}
