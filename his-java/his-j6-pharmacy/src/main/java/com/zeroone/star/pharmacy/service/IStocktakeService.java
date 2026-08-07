package com.zeroone.star.pharmacy.service;

import com.zeroone.star.project.dto.j6.pharmacy.stocktake.StocktakeAddDTO;
import com.zeroone.star.project.dto.j6.pharmacy.stocktake.StocktakeSaveRequestDTO;
import com.zeroone.star.project.vo.j6.pharmacy.stocktake.StocktakeDetailVO;

/**
 * 药库盘点单 核心业务接口。
 */
public interface IStocktakeService {

    /**
     * 新增盘点单。
     * @param dto 新增上下文（库房、盘点人等）
     * @return 完整的空白文档结构
     */
    StocktakeDetailVO createStocktake(StocktakeAddDTO dto);

    /**
     * 保存盘点单。
     * @param busNo 盘点单号
     * @param dto   保存数据（与新增返回结构一致）
     * @return 更新后的盘点单详情
     */
    StocktakeDetailVO saveStocktake(String busNo, StocktakeSaveRequestDTO dto);

    /**
     * 保存盘点单并提交审批。
     * @param busNo 盘点单号
     * @param dto   保存并提交数据（与新增返回结构一致）
     * @return 盘点单详情（状态=PENDING_APPROVAL）
     */
    StocktakeDetailVO submitStocktake(String busNo, StocktakeSaveRequestDTO dto);
}
