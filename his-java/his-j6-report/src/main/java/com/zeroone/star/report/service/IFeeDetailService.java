package com.zeroone.star.report.service;

import com.zeroone.star.project.query.j6.report.FeeDetailQuery;
import com.zeroone.star.project.vo.j6.report.OutpatientFeeDetailsVo;

public interface IFeeDetailService {

    /**
     * 分页+条件查询门诊费用列表
     * @param query 查询条件
     * @return 分页视图
     */
    OutpatientFeeDetailsVo pageFeeDetailQuery(FeeDetailQuery query);
}
