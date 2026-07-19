package com.zeroone.star.project.j4.basedata;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.query.j4.frequency.FrequencyPageQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j4.frequency.FrequencyVO;
import io.swagger.v3.oas.annotations.Operation;

/**
 * <p>
 * 描述：频次管理 API 接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
public interface FrequencyApis {

    /**
     * 获取频次列表（分页+条件）
     * @param query
     * @return
     */
    @Operation(description = "获取频次列表（分页+条件）")
    public JsonVO<PageDTO<FrequencyVO>> queryFrequencyList(FrequencyPageQuery query);

    /**
     * 获取频次详情
     * @param id
     * @return
     */
    @Operation(description = "获取频次详情")
    public JsonVO<FrequencyVO> queryFrequencyDetail(String id);
}
