package com.zeroone.star.basedata.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.basedata.entity.frequency.CusFrequencyDO;
import com.zeroone.star.project.query.j4.frequency.FrequencyPageQuery;
import com.zeroone.star.project.vo.j4.frequency.FrequencyOptionVO;
import com.zeroone.star.project.vo.j4.frequency.FrequencyVO;
import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;

import java.util.List;

/**
 * <p>
 * 描述：频次管理数据访问接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@Mapper
public interface FrequencyMapper extends BaseMapper<CusFrequencyDO> {

    /**
     * 分页+条件查询频次列表
     * @param page     分页对象
     * @param query    查询条件
     * @param tenantId 当前租户ID
     * @return 频次分页数据
     */
    Page<FrequencyVO> selectFrequencyPage(
            Page<FrequencyVO> page,
            @Param("query") FrequencyPageQuery query,
            @Param("tenantId") String tenantId
    );

    /**
     * 根据ID查询频次详情（限定当前租户）
     * @param id       频次ID
     * @param tenantId 当前租户ID
     * @return 频次详情
     */
    FrequencyVO selectFrequencyById(@Param("id") String id, @Param("tenantId") String tenantId);

    /**
     * 获取已启用的频次名称下拉选项（限定当前租户）
     * @param tenantId 当前租户ID
     * @return 频次下拉选项列表
     */
    List<FrequencyOptionVO> selectActiveFrequencyOptions(@Param("tenantId") String tenantId);
}
