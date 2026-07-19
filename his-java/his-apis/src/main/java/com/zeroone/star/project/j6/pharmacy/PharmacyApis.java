package com.zeroone.star.project.j6.pharmacy;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.query.j6.pharmacy.StockOutQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j6.pharmacy.StockOutDetailVO;
import com.zeroone.star.project.vo.j6.pharmacy.StockOutListVO;

public interface PharmacyApis {

    /**
     * 删除出库单
     * @param id 出库单ID
     * @return 是否成功
     */
    JsonVO<String> removeStockOut(String id);

    /**
     * 获取出库单详情
     * @param id 出库单ID
     * @return 详情
     */
    JsonVO<StockOutDetailVO> getStockOutDetail(String id);

    /**
     * 分页、条件查询出库单列表
     * @param stockOutQuery 查询条件
     * @return 分页结果
     */
    JsonVO<PageDTO<StockOutListVO>> getPharmacy(StockOutQuery stockOutQuery);
}
