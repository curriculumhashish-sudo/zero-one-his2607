package com.zeroone.star.pharmacy.service;

import com.zeroone.star.pharmacy.entity.WkfSupplyRequest;
import com.baomidou.mybatisplus.extension.service.IService;
import com.zeroone.star.project.dto.j7.pharmacy.approval.ApproveDTO;
import com.zeroone.star.project.dto.j7.pharmacy.approval.OperatorDTO;
import com.zeroone.star.project.vo.j7.pharmacy.StocktakeStatusVO;

/**
 * <p>
 * 工作流 supply request 服务类
 * </p>
 *
 * @author tsfmn
 * @since 2026-07-22
 */
public interface IWkfSupplyRequestService extends IService<WkfSupplyRequest> {

    StocktakeStatusVO submit(OperatorDTO dto);

    StocktakeStatusVO approve(ApproveDTO dto);

    StocktakeStatusVO reject(ApproveDTO dto);

    StocktakeStatusVO withdraw(OperatorDTO dto);
}
