package com.zeroone.star.project.j7.pharmacy.approval;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j7.pharmacy.approval.ApproveDTO;
import com.zeroone.star.project.dto.j7.pharmacy.approval.OperatorDTO;
import com.zeroone.star.project.dto.j7.pharmacy.approval.PharmacyStockTakeDTO;
import com.zeroone.star.project.query.j7.pharmacy.approval.StockTakeQuery;
import com.zeroone.star.project.vo.j7.pharmacy.approval.StocktakeStatusVO;
import com.zeroone.star.project.vo.JsonVO;

public interface PharmacyStockTakeApis {

    /**
     * 根据聚合单号查询所有盘点单明细列表
     */
    JsonVO<PageDTO<PharmacyStockTakeDTO>> getPharmacyStockTakeList(StockTakeQuery query);

    /**
     * 审批状态通过
     */
    JsonVO<StocktakeStatusVO> approvePharmacyStockTake(ApproveDTO dto);
    /**
     * 驳回审批
     */
    JsonVO<StocktakeStatusVO> rejectPharmacyStockTake(ApproveDTO dto);
    /**
     * 撤回审批
     */
    JsonVO<StocktakeStatusVO> withdrawPharmacyStockTake(OperatorDTO dto);
}
