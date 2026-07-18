package com.zeroone.star.project.j6.pharmacy;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j6.pharmacy.StockOutApprovalDTO;
import com.zeroone.star.project.dto.j6.pharmacy.StockOutDeleteDTO;
import com.zeroone.star.project.dto.j6.pharmacy.StockOutSaveDTO;
import com.zeroone.star.project.query.j6.pharmacy.StockOutDetailQuery;
import com.zeroone.star.project.query.j6.pharmacy.StockOutQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j6.pharmacy.StockOutDetailVO;
import com.zeroone.star.project.vo.j6.pharmacy.StockOutListVO;

/**
 * <p>
 * 描述：药库管理API接口定义（出库单CRUD + 出库审批流转）
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j6小组-告别
 * @version 1.0.0
 */
public interface PharmacyApis {

    // ==================== 出库单 CRUD ====================

    /**
     * 新增/编辑出库单
     * @param dto 出库单保存数据（busNo为空时新增，busNo非空时编辑）
     * @return 出库单号
     */
    JsonVO<String> saveStockOut(StockOutSaveDTO dto);

    /**
     * 删除出库单
     * @param dto 删除参数（仅PENDING_APPROVAL/REJECT/WITHDRAW状态可删）
     * @return 删除结果
     */
    JsonVO<String> deleteStockOut(StockOutDeleteDTO dto);

    /**
     * 分页查询出库单列表
     * @param condition 查询条件
     * @return 分页结果
     */
    JsonVO<PageDTO<StockOutListVO>> queryStockOuts(StockOutQuery condition);

    /**
     * 查询出库单详情（含明细行）
     * @param query 查询条件（busNo）
     * @return 出库单详情
     */
    JsonVO<StockOutDetailVO> queryStockOutDetail(StockOutDetailQuery query);

    // ==================== 出库审批流转 ====================

    /**
     * 出库单审批操作（提交/撤回/同意/驳回四合一）
     * @param dto 审批参数（action: SUBMIT/WITHDRAW/AGREE/REJECT）
     * @return 操作后状态 + 药房入库壳单号（AGREE时）
     */
    JsonVO<StockOutDetailVO> approveStockOut(StockOutApprovalDTO dto);
}
