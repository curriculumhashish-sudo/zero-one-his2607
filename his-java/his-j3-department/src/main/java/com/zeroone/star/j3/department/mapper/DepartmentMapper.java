package com.zeroone.star.j3.department.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.j3.department.entity.DepartmentDO;
import com.zeroone.star.j3.department.entity.DepartmentUnitDO;
import com.zeroone.star.j3.department.entity.SequenceDO;
import org.apache.ibatis.annotations.Mapper;

/**
 * <p>
 * 描述：科室管理Mapper层接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author 胡傲宇
 * @version 1.0.0
 */
@Mapper
public interface DepartmentMapper extends BaseMapper<DepartmentDO> {
    String getParentBusNo(Long parentId);

    void insertUnit(DepartmentUnitDO departmentUnitDO);

    Long getCurrentValueFromSequence(String seqKey);

    void updateCurrentValue(String seqKey, Long newValue);

    Long getTenantIdByUserId(Long currentUserId);

    void addSeqKeyFromSequence(SequenceDO sequenceDO);

    int isDoctorExist(Long defDoctorId);
}
