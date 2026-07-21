package com.zeroone.star.project.j5.configuration.orderset;

import com.zeroone.star.project.dto.j5.configuration.orderset.OrderSetAddDTO;
import com.zeroone.star.project.dto.j5.configuration.orderset.OrderSetModifyDTO;
import com.zeroone.star.project.dto.j5.configuration.orderset.OrderSetStatusDTO;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j5.configuration.orderset.OrderSetOperationVO;

/**
 * 配置管理-组套模板API
 */
public interface OrderSetApis {

    /**
     * 新增组套模板。
     * @param dto 新增数据
     * @return 操作结果
     */
    JsonVO<OrderSetOperationVO> addOrderSet(OrderSetAddDTO dto);

    /**
     * 修改组套模板。
     * @param dto 修改数据
     * @return 操作结果
     */
    JsonVO<OrderSetOperationVO> modifyOrderSet(OrderSetModifyDTO dto);

    /**
     * 删除组套模板。
     * @param id 组套模板编号
     * @return 被删除的组套模板编号
     */
    JsonVO<String> removeOrderSet(String id);

    /**
     * 启用或停用组套模板。
     * @param dto 状态操作数据
     * @return 操作结果
     */
    JsonVO<OrderSetOperationVO> modifyOrderSetStatus(OrderSetStatusDTO dto);
}
