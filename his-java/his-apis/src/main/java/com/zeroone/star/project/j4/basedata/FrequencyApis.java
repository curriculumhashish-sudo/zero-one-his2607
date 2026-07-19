package com.zeroone.star.project.j4.basedata;

import com.zeroone.star.project.dto.PageDTO;

import com.zeroone.star.project.dto.j4.frequency.FrequencySaveDTO;
import com.zeroone.star.project.dto.j4.frequency.FrequencyUpdateDTO;
import com.zeroone.star.project.dto.j4.frequency.FrequencyStatusDTO;
import com.zeroone.star.project.query.j4.frequency.FrequencyPageQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j4.frequency.FrequencyOptionVO;
import com.zeroone.star.project.vo.j4.frequency.FrequencyVO;

import java.util.List;

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
    public JsonVO<PageDTO<FrequencyVO>> queryFrequencyList(FrequencyPageQuery query);

    /**
     * 获取频次详情
     * @param id
     * @return
     */
    public JsonVO<FrequencyVO> queryFrequencyDetail(String id);

    /**
     * 新增频次接口
     * @param dto 新增数据
     * @return 展现层数据结果
     */
    JsonVO<FrequencyVO> addFrequency(FrequencySaveDTO dto);

    /**
     * 修改频次接口
     * @param dto 修改数据
     * @return 展现层数据结果
     */
    JsonVO<FrequencyVO> modifyFrequency(FrequencyUpdateDTO dto);

    /**
     * 删除频次接口
     * @param id 频次主键
     * @return 结果状态载荷
     */
    JsonVO<Void> removeFrequency(String id);
     * 获取可用频次名称列表
     *
     * @return 频次名称列表
     */
    JsonVO<List<FrequencyOptionVO>> listFrequencyNames();

    /**
     * 启用或停用频次
     *
     * @param dto 频次状态参数
     * @return 是否修改成功
     */
    JsonVO<Boolean> modifyFrequencyStatus(FrequencyStatusDTO dto);
}
