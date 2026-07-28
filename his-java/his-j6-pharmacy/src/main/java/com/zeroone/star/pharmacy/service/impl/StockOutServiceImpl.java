package com.zeroone.star.pharmacy.service.impl;

import com.zeroone.star.pharmacy.service.IStockOutService;
import com.zeroone.star.project.dto.j6.pharmacy.stockout.StockOutSaveDTO;
import com.zeroone.star.project.dto.j6.pharmacy.stockout.SubmitStockOutApprovalDTO;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j6.pharmacy.stockout.StockOutApprovalResultVO;
import com.zeroone.star.project.vo.j6.pharmacy.stockout.StockOutSaveResultVO;
import lombok.RequiredArgsConstructor;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.time.LocalDate;
import java.time.LocalDateTime;

/**
 * 药库出库单服务实现
 */
@Service
@RequiredArgsConstructor
public class StockOutServiceImpl implements IStockOutService {

    // 状态常量
    private static final String STATUS_DRAFT = "DRAFT";
    private static final String STATUS_PENDING_APPROVAL = "PENDING_APPROVAL";
    private static final String STATUS_APPROVED = "APPROVED";
    private static final String STATUS_REJECTED = "REJECTED";

    // TODO: 后续注入 Mapper
    // private final StockOutMapper stockOutMapper;

    @Override
    public JsonVO<StockOutApprovalResultVO> saveAndSubmitApproval(StockOutSaveDTO dto) {
        // 第一步：保存出库单
        StockOutSaveResultVO saveResult = saveStockOut(dto).getData();

        // 第二步：提交审批
        SubmitStockOutApprovalDTO approvalDTO = new SubmitStockOutApprovalDTO();
        approvalDTO.setBusNo(saveResult.getBusNo());
        // approvalDTO.setApproverId(...); // 如果需要审批人ID

        StockOutApprovalResultVO result = new StockOutApprovalResultVO();
        result.setSuccess(true);
        result.setStatusEnum("APPROVAL");

        return JsonVO.success(result);
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public JsonVO<StockOutSaveResultVO> saveStockOut(StockOutSaveDTO dto) {
        StockOutSaveResultVO result = new StockOutSaveResultVO();
        result.setBusNo(generateBusNo());  // 只设置 busNo
        // 其他字段不设置，默认为 null

        return JsonVO.success(result);
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public JsonVO<StockOutApprovalResultVO> submitApproval(SubmitStockOutApprovalDTO dto) {
        StockOutApprovalResultVO result = new StockOutApprovalResultVO();

        // TODO: 业务验证
        // 1. 检查单据是否存在
        // 2. 检查当前状态是否允许提交审批
        // 3. 检查明细行是否完整

        // TODO: 更新数据库状态
        // stockOutMapper.updateStatus(dto.getBusNo(), STATUS_PENDING_APPROVAL);

        result.setSuccess(true);
        result.setStatusEnum("APPROVAL");

        return JsonVO.success(result);
    }

    /**
     * 生成单据号
     */

    // 使用静态变量模拟自增（线程不安全，仅供演示）
    private static int counter = 1;

    private String generateBusNo() {
        LocalDate today = LocalDate.now();

        // 格式：CSOyyyy-MM-ddXXXX
        String busNo = String.format("CSO%d-%02d-%02d%04d",
                today.getYear(),
                today.getMonthValue(),
                today.getDayOfMonth(),
                counter++);

        return busNo;
    }
}