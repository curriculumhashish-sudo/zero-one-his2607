package com.zeroone.star.basedata.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.basedata.entity.treatment.WkfActivityDefinitionDO;
import com.zeroone.star.project.query.j4.treatment.TreatmentQuery;
import com.zeroone.star.project.vo.j4.treatment.TreatmentListVO;
import org.apache.ibatis.annotations.Mapper;

/**
 * <p>
 * 描述：诊疗管理数据访问接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@Mapper
public interface TreatmentMapper extends BaseMapper<WkfActivityDefinitionDO> {
    Page<TreatmentListVO> selectTreatmentListPage(Page<TreatmentListVO> page, TreatmentQuery query);
}
