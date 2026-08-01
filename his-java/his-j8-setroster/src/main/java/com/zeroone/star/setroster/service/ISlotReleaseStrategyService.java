package com.zeroone.star.setroster.service;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j8.setroster.SlotReleaseStrategyAddDTO;
import com.zeroone.star.project.dto.j8.setroster.SlotReleaseStrategyDTO;
import com.zeroone.star.project.dto.j8.setroster.SlotReleaseStrategyModifyDTO;
import com.zeroone.star.project.dto.j8.setroster.SlotReleaseStrategyNameDTO;
import com.zeroone.star.project.query.j8.setroster.SlotReleaseStrategyQuery;

import java.util.List;

/**
 * 放号策略业务服务。
 */
public interface ISlotReleaseStrategyService {

    PageDTO<SlotReleaseStrategyDTO> listStrategies(SlotReleaseStrategyQuery query);

    List<SlotReleaseStrategyNameDTO> listStrategyNames();

    String saveStrategy(SlotReleaseStrategyAddDTO addDTO);

    boolean updateStrategy(SlotReleaseStrategyModifyDTO modifyDTO);

    boolean removeStrategy(String id);
}
