package com.zeroone.star.j3.department.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.j3.department.entity.DepartmentDO;
import com.zeroone.star.j3.department.entity.DepartmentUnitDO;
import com.zeroone.star.j3.department.entity.SequenceDO;
import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;

import java.time.LocalDateTime;

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
    DepartmentDO selectUpdateDepartmentById(@Param("id") String id);

    String getParentBusNo(Long parentId);

    void insertUnit(DepartmentUnitDO departmentUnitDO);

    Long getCurrentValueFromSequence(String seqKey);

    void updateCurrentValue(String seqKey, Long newValue);

    Long getTenantIdByUserId(Long currentUserId);

    void addSeqKeyFromSequence(SequenceDO sequenceDO);

    int isDoctorExist(Long defDoctorId);

    Long selectParentUnitIdByBusNo(String parentBusNO);

    int isDoctorAvailable(@Param("defDoctorId") String defDoctorId,
                          @Param("tenantId") String tenantId);

    Long selectUnitIdByBusNo(@Param("busNo") String busNo,
                             @Param("tenantId") String tenantId);

    /**
     * 按科室业务编码同步组织单元的可编辑信息。
     *
     * @return 更新行数
     */
    int updateUnitByCode(@Param("code") String code,
                         @Param("displayName") String displayName,
                         @Param("sortOrder") Integer sortOrder,
                         @Param("parentIdChanged") boolean parentIdChanged,
                         @Param("parentId") Long parentId,
                         @Param("updatedAt") LocalDateTime updatedAt,
                         @Param("updatedBy") Long updatedBy,
                         @Param("tenantId") String tenantId);
}
