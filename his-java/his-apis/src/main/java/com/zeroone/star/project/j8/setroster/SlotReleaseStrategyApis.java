package com.zeroone.star.project.j8.setroster;


import com.zeroone.star.project.dto.j8.setroster.SlotReleaseStrategyAddDTO;
import com.zeroone.star.project.dto.j8.setroster.SlotReleaseStrategyDTO;
import com.zeroone.star.project.dto.j8.setroster.SlotReleaseStrategyIdVO;
import com.zeroone.star.project.dto.j8.setroster.SlotReleaseStrategyModifyDTO;
import com.zeroone.star.project.dto.j8.setroster.SlotReleaseStrategyPageVO;
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
     * 获取放号策略列表（条件+分页）
     * @param query 查询条件
     * @return 放号策略分页列表
     */
    JsonVO<SlotReleaseStrategyPageVO<SlotReleaseStrategyDTO>> querySlotReleaseStrategies(
            SlotReleaseStrategyQuery query);

    /**
     * 获取放号策略名称
     * @param query 查询条件
     * @return 放号策略分页列表
     */
    JsonVO<SlotReleaseStrategyPageVO<SlotReleaseStrategyDTO>> querySlotReleaseStrategyNames(
            SlotReleaseStrategyQuery query);

    /**
     * 新建放号策略
     * @param addDTO 新建参数
     * @return 新建结果（含ID）
     */
    JsonVO<SlotReleaseStrategyIdVO> addSlotReleaseStrategy(SlotReleaseStrategyAddDTO addDTO);

    /**
     * 修改放号策略
     * @param modifyDTO 修改参数
     * @return 修改结果（含ID）
     */
    JsonVO<SlotReleaseStrategyIdVO> modifySlotReleaseStrategy(SlotReleaseStrategyModifyDTO modifyDTO);

    /**
     * 删除放号策略
     * @param id 策略ID
     * @return 删除结果
     */
    JsonVO<String> deleteSlotReleaseStrategy(String id);
}