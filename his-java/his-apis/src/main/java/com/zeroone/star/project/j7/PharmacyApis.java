package com.zeroone.star.project.j7;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j7.pharmacy.*;
import com.zeroone.star.project.dto.j7.pharmacy.approval.ApproveDTO;
import com.zeroone.star.project.dto.j7.pharmacy.approval.OperatorDTO;
import com.zeroone.star.project.query.j7.pharmacy.PatientQueueQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j7.pharmacy.DrugItemVO;
import com.zeroone.star.project.vo.j7.pharmacy.InboundApprovalVO;
import com.zeroone.star.project.vo.j7.pharmacy.PatientQueueVO;
import com.zeroone.star.project.vo.j7.pharmacy.StocktakeStatusVO;
import com.zeroone.star.project.vo.j7.pharmacy.SummaryVO;

import java.util.List;

public interface PharmacyApis {

        /**
         * G10 盘点单审批接口
         * @author tsfmn
         * @version 1.0.0
         * a
         */
        /**
         * 盘点单提交审批
         */
        JsonVO<StocktakeStatusVO> submit(OperatorDTO dto);

        /**
         * 盘点单审批状态通过
         */
        JsonVO<StocktakeStatusVO> approve(ApproveDTO dto);
        /**
         * 盘点单驳回审批
         */
        JsonVO<StocktakeStatusVO> reject(ApproveDTO dto);
        /**
         * 盘点单撤回审批
         */
        JsonVO<StocktakeStatusVO> withdraw(OperatorDTO dto);



        /**
         * G7 入库单审批流转接口。
         *
         * @author lvv
         * @version 1.0.0
         */

        /**
         * 撤回待审批的入库单。
         *
         * @param dto 入库单审批流转请求参数
         * @return 操作结果
         */
        JsonVO<InboundApprovalVO> withdrawApproval(InboundApprovalDTO dto);
        /**
         * 审批通过入库单。
         *
         * @param dto 入库单审批流转请求参数
         * @return 操作结果
         */
        JsonVO<InboundApprovalVO> approveApproval(InboundApprovalDTO dto);
        /**
         * 驳回入库单审批。
         *
         * @param dto 入库单审批流转请求参数
         * @return 操作结果
         */
        JsonVO<InboundApprovalVO> rejectApproval(InboundApprovalDTO dto);



    /**
     * <p>
     * 描述：住院药房API接口定义
     * </p>
     * <p>
     * 功能说明：定义住院药房对外提供的 RESTful API 接口规范，
     * 包括住院发药驳回和住院退药确认两大业务模块：
     * 1. 住院发药驳回：获取汇总单队列 → 查看药品项目 → 发药核对 → 驳回发药
     * 2. 住院退药确认：查询退药患者队列 → 查看退药明细 → 确认退药
     * 该接口由 his-pharmacy 模块的 InpatientController 实现。
     * </p>
     * <p>版权：&copy;01星球</p>
     * <p>地址：01星球总部</p>
     * @author 01star
     * @version 1.0.0
     */

        // ==================== 住院发药驳回 ====================

        /**
         * 获取汇总单队列
         * @return 汇总单列表
         */
        JsonVO<List<SummaryVO>> getSummaryList();

        /**
         * 根据汇总单ID获取患者药品项目
         * @param summaryId 汇总单ID
         * @return 药品项目列表
         */
        JsonVO<List<DrugItemVO>> getDrugItemsBySummary(String summaryId);

        /**
         * 执行发药核对（发药后截单）
         * @param dispensingId 发药单据ID
         * @return 操作结果
         */
        JsonVO<String> confirmDispensing(String dispensingId);

        /**
         * 驳回患者发药单据
         * @param dto 驳回参数
         * @return 操作结果
         */
        JsonVO<String> rejectDispensing(DispensingRejectDTO dto);

        // ==================== 住院退药确认 ====================

        /**
         * 分页查询退药患者队列
         * @param query 分页查询参数
         * @return 患者队列分页数据
         */
        JsonVO<PageDTO<PatientQueueVO>> queryReturnPatientQueue(PatientQueueQuery query);

        /**
         * 获取患者退药明细信息
         * @param returnId 退药申请单ID
         * @return 药品明细列表
         */
        JsonVO<List<DrugItemVO>> getReturnDrugItems(String returnId);

        /**
         * 确认退药（对勾选药品执行退药，同步回补药品库存）
         * @param dto 退药确认参数
         * @return 操作结果
         */
        JsonVO<String> confirmReturn(ReturnConfirmDTO dto);




    /**
     * <p>
     * 描述：门诊药房API接口定义
     * </p>
     * <p>
     * 功能说明：定义门诊药房对外提供的 RESTful API 接口规范，
     * 包括门诊拒绝退药业务模块：
     * 1. 分页查询门诊退药患者队列
     * 2. 查看患者退药明细
     * 3. 执行拒绝退药
     * 该接口由 his-pharmacy 模块的 OutpatientController 实现。
     * </p>
     * <p>版权：&copy;01星球</p>
     * <p>地址：01星球总部</p>
     * @author 01star
     * @version 1.0.0
     */


        // ==================== 门诊拒绝退药 ====================

        /**
         * 分页查询门诊退药患者队列
         * @param query 分页查询参数
         * @return 患者队列分页数据
         */
        JsonVO<PageDTO<PatientQueueVO>> queryOutpatientReturnQueue(PatientQueueQuery query);



        /**
         * 执行拒绝退药
         * @param dto 拒绝退药参数
         * @return 操作结果
         */
        JsonVO<String> rejectReturn(ReturnRejectDTO dto);






        /**
         * <p>
         * 描述：药房入库管理 API 接口定义
         * </p>
         * <p>
         * 功能说明：定义药房入库单相关的 RESTful API 接口规范，
         * 包括新增入库单、编辑保存草稿、保存并提交审批等功能：
         * 1. 新增入库单据：创建草稿态入库单，自动计算金额
         * 2. 保存入库单据：全量更新草稿单据的主表及明细
         * 3. 保存并提交审批：保存后立即将状态变更为“待审批”
         * 该接口由 his-pharmacy 模块的 InOrderController 实现。
         * </p>
         * <p>版权：&copy;01星球</p>
         * <p>地址：01星球总部</p>
         * @author 01star
         * @version 1.0.0
         */

        // ==================== 药房入库管理 ====================

        /**
         * 新增入库单据
         * <p>
         * 创建一张新的药房入库单，初始状态为“草稿”。系统根据传入的数量、单价自动计算
         * 明细行合计金额和整单总金额。入库单号可手动传入或由后端自动生成。
         * </p>
         * @param dto 入库单数据传输对象，包含主表信息及药品明细列表
         * @return 操作结果，data 中包含新创建的入库单主键 ID
         */
        JsonVO<String> addInOrder(InStockOrderDTO dto);

        /**
         * 保存入库单据
         * <p>
         * 对已存在的草稿入库单进行全量编辑保存。主表字段直接覆盖，明细采用全量替换策略
         * （先删除原有明细，再插入新明细），并重新汇总计算整单总金额。
         * 仅允许状态为“草稿”的单据执行此操作。
         * </p>
         * @param orderId 入库单主键 ID
         * @param dto     更新后的入库单数据
         * @return 操作结果
         */
        JsonVO<String> updateInOrder(Long orderId, InStockOrderDTO dto);

        /**
         * 保存并提交审批
         * <p>
         * 在完成保存逻辑后，立即将单据状态从“草稿”变更为“待审批”。
         * 该操作要求单据当前必须处于草稿状态，否则抛出业务异常。
         * 适用于最终确认后直接提交审核的场景。
         * </p>
         * @param orderId 入库单主键 ID
         * @param dto     最终版本的入库单数据
         * @return 操作结果
         */
        JsonVO<String> submitInOrder(Long orderId, InStockOrderDTO dto);

}
