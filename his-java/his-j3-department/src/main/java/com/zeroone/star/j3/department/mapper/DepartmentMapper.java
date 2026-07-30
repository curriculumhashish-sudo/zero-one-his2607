package com.zeroone.star.j3.department.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.j3.department.entity.DepartmentDO;
import com.zeroone.star.j3.department.entity.DepartmentUnitDO;
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
    DepartmentDO selectUpdateDepartmentById(@Param("id") String id,
                                            @Param("tenantId") String tenantId);

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

    int isDoctorAvailable(@Param("defDoctorId") String defDoctorId,
                          @Param("tenantId") String tenantId);

    Long selectUnitIdByBusNo(@Param("busNo") String busNo,
                             @Param("tenantId") String tenantId);

    int countUndeletedChildDepartments(@Param("parentId") String parentId,
                                       @Param("tenantId") String tenantId);

    int existsDeleteBusinessReference(@Param("departmentId") String departmentId,
                                      @Param("tenantId") String tenantId);

    int softDeleteDepartment(@Param("id") String id,
                             @Param("tenantId") String tenantId,
                             @Param("version") Integer version,
                             @Param("deletedAt") LocalDateTime deletedAt,
                             @Param("deletedBy") String deletedBy);

    int softDeleteUnitByCode(@Param("code") String code,
                             @Param("tenantId") String tenantId,
                             @Param("deletedAt") LocalDateTime deletedAt,
                             @Param("deletedBy") String deletedBy);

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

    /**
     * 检查序列键是否存在
     *
     * @param seqKey 序列键
     * @return 序列键是否存在
     * @author 胡傲宇
     */
    boolean isSeqKeyExist(String seqKey);

    /**
     * 创建序列
     *
     * @param seqKey 序列键
     * @author 胡傲宇
     */
    void createSequence(String seqKey);
}
