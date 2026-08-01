package com.zeroone.star.pharmacy.service;

import com.zeroone.star.project.dto.j6.pharmacy.stocktake.StocktakeSaveDTO;
import com.zeroone.star.project.vo.j6.pharmacy.stocktake.StocktakeDetailVO;

/**
 * 药库盘点单 核心业务接口
 * <p>负责盘点单的创建、保存、保存并提交审批</p>
 *
 * @author Dome
 * @since 2026-07-24
 */
public interface IStocktakeService {

    /**
     * 新增盘点单
     * <p>生成空盘点单头，不生成明细行。明细由前端逐行添加后通过 saveStocktake 保存</p>
     *
     * @param dto 头信息（purposeLocationId、categoryEnum 必填，busNo 由服务端生成）
     * @return 空明细行的盘点单详情（lines=[]）
     */
    StocktakeDetailVO createStocktake(StocktakeSaveDTO dto);

    /**
     * 保存盘点单
     * <p>先删后插策略：快照锁定账面数 → 补全物品信息 → 批量写入明细行</p>
     *
     * @param busNo 盘点单号
     * @param dto   头信息 + 明细行列表（明细行仅含用户输入字段）
     * @return 完整补全的盘点单详情
     */
    StocktakeDetailVO saveStocktake(String busNo, StocktakeSaveDTO dto);

    /**
     * 保存盘点单并提交审批
     * <p>保存盘点数据 + 状态流转 DRAFT → PENDING_APPROVAL</p>
     *
     * @param busNo 盘点单号
     * @param dto   头信息 + 明细行列表
     * @return 盘点单详情（状态=PENDING_APPROVAL）
     */
    StocktakeDetailVO submitStocktake(String busNo, StocktakeSaveDTO dto);
}
