package com.zeroone.star.pharmacy.service;

import com.baomidou.mybatisplus.extension.service.IService;
import com.zeroone.star.pharmacy.entity.WkfSupplyRequest;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.stocktake.StocktakeSaveDTO;
import com.zeroone.star.project.dto.stocktake.StocktakeSubmitDTO;
import com.zeroone.star.project.query.stocktake.StocktakeQuery;
import com.zeroone.star.project.vo.stocktake.StocktakeDetailVO;
import com.zeroone.star.project.vo.stocktake.StocktakeSaveVO;

/**
 * <p>
 * 描述：盘点单查询提交服务接口
 * </p>
 * <p>
 * 功能说明：基于 {@code wkf_supply_request} 表（{@code type_enum = INVENTORY_CHECK}）
 * 提供盘点单的查询、详情查看、提交审批三大业务能力：
 * 1. 分页查询盘点单列表（按 bus_no 聚合）
 * 2. 查询盘点单详情（含汇总信息 + 明细列表）
 * 3. 提交盘点单审批（DRAFT → PENDING_APPROVAL，批量更新同一 bus_no 下所有行）
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 阿伟学长
 * @version 1.0.0
 */
public interface IStocktakeService extends IService<WkfSupplyRequest> {

    /**
     * 新增盘点单草稿信息。
     * <p>
     * 当前表结构不允许插入空单据，因此新增接口只生成草稿单号和默认状态，
     * 真正落库发生在保存接口。
     * </p>
     *
     * @return 草稿信息
     */
    StocktakeSaveVO addStocktake();

    /**
     * 分页查询盘点单列表
     * @param query 查询条件（含分页参数）
     * @return 分页数据
     */
    PageDTO<com.zeroone.star.project.vo.stocktake.StocktakeListVO> queryStocktakeList(StocktakeQuery query);

    /**
     * 查询盘点单详情（含明细列表）
     * @param id 盘点单号（bus_no）
     * @return 盘点单详情（含 items 明细列表）；不存在返回 null
     */
    StocktakeDetailVO queryStocktakeDetail(String id);

    /**
     * 保存盘点单草稿。
     *
     * @param dto 保存参数
     * @return 保存结果
     */
    StocktakeSaveVO saveStocktake(StocktakeSaveDTO dto);

    /**
     * 保存并提交盘点单审批。
     *
     * @param dto 保存参数
     * @return 保存结果
     */
    StocktakeSaveVO saveAndSubmitStocktake(StocktakeSaveDTO dto);

    /**
     * 提交盘点单审批
     * <p>
     * 业务校验规则：
     * 1. 单据（bus_no）必须存在且 type_enum = INVENTORY_CHECK
     * 2. status_enum 必须为 DRAFT（草稿状态）
     * 3. 盘点单至少有一条明细行
     * </p>
     * <p>
     * 状态变更：DRAFT → PENDING_APPROVAL，同时写入 applicant_id、apply_time
     * </p>
     *
     * @param dto 提交审批参数（含盘点单ID/备注）
     * @return 提交结果消息
     * @throws IllegalArgumentException 业务校验失败时抛出
     */
    String submitStocktake(StocktakeSubmitDTO dto);
}
