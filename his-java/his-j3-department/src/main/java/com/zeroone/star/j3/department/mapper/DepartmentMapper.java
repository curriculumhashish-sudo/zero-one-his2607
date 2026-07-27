package com.zeroone.star.j3.department.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.j3.department.entity.DepartmentDO;
import com.zeroone.star.j3.department.entity.DepartmentUnitDO;
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
    /**
     * 根据父ID查询父部门的业务编号
     *
     * @param parentId 父部门ID
     * @return 父部门的业务编号
     * @author 胡傲宇
     */
    String getParentBusNo(Long parentId);

    /**
     * 插入科室单位关联记录
     *
     * @param departmentUnitDO 科室单位关联记录
     * @author 胡傲宇
     */
    void insertUnit(DepartmentUnitDO departmentUnitDO);

    /**
     * 根据用户ID查询租户ID
     *
     * @param currentUserId 当前用户ID
     * @return 租户ID
     * @author 胡傲宇
     */
    Long getTenantIdByUserId(Long currentUserId);

    /**
     * 检查医生是否存在
     *
     * @param defDoctorId 医生ID
     * @return 医生是否存在
     * @author 胡傲宇
     */
    int isDoctorExist(Long defDoctorId);

    /**
     * 根据父业务编号查询父单位ID
     *
     * @param parentBusNO 父业务编号
     * @return 父单位ID
     * @author 胡傲宇
     */
    Long selectParentUnitIdByBusNo(String parentBusNO);

    /**
     * 增加序列值
     *
     * @param seqKey 序列键
     * @author 胡傲宇
     */
    void incrementSequence(String seqKey);

    /**
     * 获取当前序列值
     *
     * @return 当前序列值
     * @author 胡傲宇
     */
    Long getCurrentValueAfterIncrement();
}
