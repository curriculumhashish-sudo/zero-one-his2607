package com.zeroone.star.project.j8.setroster;


import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j8.setroster.SlotReleaseStrategyAddDTO;
import com.zeroone.star.project.dto.j8.setroster.SlotReleaseStrategyDTO;
import com.zeroone.star.project.dto.j8.setroster.SlotReleaseStrategyDetailDTO;
import com.zeroone.star.project.dto.j8.setroster.SlotReleaseStrategyModifyDTO;
import com.zeroone.star.project.query.j8.setroster.SlotReleaseStrategyQuery;
import com.zeroone.star.project.vo.JsonVO;

/**
 * <p>
 * 描述：放号策略接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author 余悸
 * @version 1.0.0
 */

public interface SlotReleaseStrategyApis {
    /**
     * 查询所有放号策略
     * @param query 查询条件
     * @return 放号策略列表
     */
    JsonVO<PageDTO<SlotReleaseStrategyDTO>>querySlotReleaseStrategies(SlotReleaseStrategyQuery query);


    /**
     * 查询放号策略详情
     * @param id 查询条件
     * @return 放号策略详情
     */
    JsonVO<SlotReleaseStrategyDetailDTO> getSlotReleaseStrategy(Long id);





    /**
     * 添加放号策略
     * @param addDTO 添加条件
     * @return 添加结果
     */
    JsonVO<String> addSlotReleaseStrategy(SlotReleaseStrategyAddDTO addDTO);


    /**
     * 修改放号策略
      * @param modifyDTO 修改条件
     * @return 修改结果
     */
    JsonVO<String> modifySlotReleaseStrategy(
        SlotReleaseStrategyModifyDTO modifyDTO);


    /**
     * 删除放号策略
     * @param id 删除条件
     * @return 删除结果
     */
    JsonVO<String> deleteSlotReleaseStrategy(Long id);

}
