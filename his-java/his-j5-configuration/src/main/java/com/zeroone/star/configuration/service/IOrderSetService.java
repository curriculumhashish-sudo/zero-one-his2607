package com.zeroone.star.configuration.service;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j5.configuration.orderset.OrderSetAddDTO;
import com.zeroone.star.project.dto.j5.configuration.orderset.OrderSetModifyDTO;
import com.zeroone.star.project.dto.j5.configuration.orderset.OrderSetStatusDTO;
import com.zeroone.star.project.query.j5.configuration.orderset.OrderSetQuery;
import com.zeroone.star.project.vo.j5.configuration.orderset.OrderSetDetailVO;
import com.zeroone.star.project.vo.j5.configuration.orderset.OrderSetListVO;
import com.zeroone.star.project.vo.j5.configuration.orderset.OrderSetNameVO;
import com.zeroone.star.project.vo.j5.configuration.orderset.OrderSetOperationVO;

import java.util.List;

/**
 * 组套模板领域服务。
 */
public interface IOrderSetService {

    /**
     * 获取组套名称列表（下拉选项）
     */
    List<OrderSetNameVO> listOrderSetNames();

    /**
     * 分页查询组套模板列表
     */
    PageDTO<OrderSetListVO> listOrderSets(OrderSetQuery query);

    /**
     * 根据ID获取组套模板详情
     */
    OrderSetDetailVO getOrderSetDetail(String id);

    /**
     * 新增组套模板
     */
    OrderSetOperationVO addOrderSet(OrderSetAddDTO dto);

    /**
     * 修改组套模板
     */
    OrderSetOperationVO modifyOrderSet(OrderSetModifyDTO dto);

    /**
     * 删除组套模板
     */
    String removeOrderSet(String id);

    /**
     * 启用或停用组套模板
     */
    OrderSetOperationVO modifyOrderSetStatus(OrderSetStatusDTO dto);
}
