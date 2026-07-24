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

/**
 * 药库出库单服务实现
 */
@Service
@RequiredArgsConstructor
public class StockOutServiceImpl implements IStockOutService {

    // 后续注入 Mapper

    @Override
    @Transactional
    public JsonVO<StockOutSaveResultVO> saveStockOut(StockOutSaveDTO dto) {
        StockOutSaveResultVO result = new StockOutSaveResultVO();

        if (dto.getBusNo() == null || dto.getBusNo().isEmpty()) {
            // 新增：生成单据号
            String busNo = "CK" + System.currentTimeMillis();
            result.setBusNo(busNo);
            // TODO: 插入数据库
        } else {
            // 编辑：更新已有单据
            result.setBusNo(dto.getBusNo());
            // TODO: 更新数据库
        }

        result.setStatus("DRAFT");
        result.setMessage("保存成功");
        result.setVersion(dto.getRowVersion() != null ? dto.getRowVersion() + 1 : 1);

        return JsonVO.success(result);
    }

    @Override
    public JsonVO<StockOutApprovalResultVO> submitApproval(SubmitStockOutApprovalDTO dto) {
        StockOutApprovalResultVO result = new StockOutApprovalResultVO();
        result.setSuccess(true);              // 设置操作成功
        result.setStatusEnum("APPROVAL");     // 设置状态枚举码

        return JsonVO.success(result);
    }
}