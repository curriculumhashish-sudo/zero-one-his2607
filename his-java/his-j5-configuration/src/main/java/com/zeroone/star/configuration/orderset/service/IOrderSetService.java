package com.zeroone.star.configuration.orderset.service;

import com.zeroone.star.project.dto.j5.configuration.orderset.OrderSetAddDTO;
import com.zeroone.star.project.dto.j5.configuration.orderset.OrderSetModifyDTO;
import com.zeroone.star.project.dto.j5.configuration.orderset.OrderSetStatusDTO;
import com.zeroone.star.project.vo.j5.configuration.orderset.OrderSetOperationVO;

/**
 * 组套模板写操作领域服务。
 */
public interface IOrderSetService {

    OrderSetOperationVO addOrderSet(OrderSetAddDTO dto);

    OrderSetOperationVO modifyOrderSet(OrderSetModifyDTO dto);

    String removeOrderSet(String id);

    OrderSetOperationVO modifyOrderSetStatus(OrderSetStatusDTO dto);
}
