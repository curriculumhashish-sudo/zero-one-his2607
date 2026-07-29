package com.zeroone.star.project.stocktake;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.stocktake.StocktakeSubmitDTO;
import com.zeroone.star.project.query.stocktake.StocktakeQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.stocktake.StocktakeDetailVO;
import com.zeroone.star.project.vo.stocktake.StocktakeListVO;

/**
 * <p>
 * 描述：盘点单查询提交接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 阿伟学长
 * @version 1.0.0
 */
public interface StocktakeApis {

    /**
     * 获取盘点单列表（分页查询）
     * @param query 查询条件
     * @return 盘点单分页列表
     */
    JsonVO<PageDTO<StocktakeListVO>> queryStocktakeList(StocktakeQuery query);

    /**
     * 获取盘点单详情
     * @param id 盘点单ID
     * @return 盘点单详情
     */
    JsonVO<StocktakeDetailVO> queryStocktakeDetail(String id);

    /**
     * 提交盘点单审批
     * @param dto 提交审批数据
     * @return 提交结果
     */
    JsonVO<String> submitStocktake(StocktakeSubmitDTO dto);
}
