package com.zeroone.star.basedata.service;

import com.baomidou.mybatisplus.extension.service.IService;
import com.zeroone.star.basedata.entity.Frequency;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j4.frequency.FrequencyStatusDTO;
import com.zeroone.star.project.query.j4.frequency.FrequencyPageQuery;
import com.zeroone.star.project.vo.j4.frequency.FrequencyOptionVO;
import com.zeroone.star.project.vo.j4.frequency.FrequencyVO;

import java.util.List;

/**
 * <p>
 * 描述：频次管理服务接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
public interface IFrequencyService extends IService<Frequency> {

    /**
     * 分页+条件查询频次列表
     * @param query 分页查询条件
     * @return 分页结果
     */
    PageDTO<FrequencyVO> queryFrequencyPage(FrequencyPageQuery query);

    /**
     * 根据ID查询频次详情
     * @param id 频次ID
     * @return 频次详情
     */
    FrequencyVO queryFrequencyDetail(String id);

    /**
     * 获取已启用的频次名称列表
     *
     * @return 频次下拉选项列表
     */
    List<FrequencyOptionVO> listFrequencyNames();

    /**
     * 修改频次启停状态
     *
     * @param dto 频次状态参数
     * @return 是否修改成功
     */
    boolean modifyFrequencyStatus(FrequencyStatusDTO dto);
}
