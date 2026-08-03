package com.zeroone.star.setroster.service;

import com.zeroone.star.project.dto.j8.setroster.SlotReleaseStrategyAddDTO;
import com.zeroone.star.project.dto.j8.setroster.SlotReleaseStrategyDTO;
import com.zeroone.star.project.dto.j8.setroster.SlotReleaseStrategyIdVO;
import com.zeroone.star.project.dto.j8.setroster.SlotReleaseStrategyModifyDTO;
import com.zeroone.star.project.dto.j8.setroster.SlotReleaseStrategyPageVO;
import com.zeroone.star.project.query.j8.setroster.SlotReleaseStrategyQuery;

/**
 * 放号策略业务服务。
 */
public interface ISlotReleaseStrategyService {

    SlotReleaseStrategyPageVO<SlotReleaseStrategyDTO> listStrategies(SlotReleaseStrategyQuery query);

    SlotReleaseStrategyPageVO<SlotReleaseStrategyDTO> listStrategyNames(SlotReleaseStrategyQuery query);

    SlotReleaseStrategyIdVO saveStrategy(SlotReleaseStrategyAddDTO addDTO);

    SlotReleaseStrategyIdVO updateStrategy(SlotReleaseStrategyModifyDTO modifyDTO);

    boolean removeStrategy(String id);
}