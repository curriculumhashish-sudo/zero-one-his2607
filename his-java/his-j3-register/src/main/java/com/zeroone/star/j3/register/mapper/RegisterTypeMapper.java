package com.zeroone.star.j3.register.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.baomidou.mybatisplus.core.metadata.IPage;
import com.zeroone.star.j3.register.entity.RegisterType;
import org.apache.ibatis.annotations.Param;

/**
 * <p>
 * 描述：挂号类型 Mapper 接口
 * </p>
 *
 * @author j3小组
 * @version 1.0.0
 */
public interface RegisterTypeMapper extends BaseMapper<RegisterType> {

    /**
     * 分页查询挂号类型（JOIN adm_healthcare_service 获取业务编码）
     * @param page 分页参数
     * @param keyword 模糊搜索关键词
     * @param tenantId 租户ID（租户隔离过滤）
     */
    IPage<RegisterType> selectRegisterTypePage(IPage<RegisterType> page,
                                                @Param("keyword") String keyword,
                                                @Param("tenantId") String tenantId);

    /**
     * 根据名称查找 healthcare_service 的 id（新增时自动关联，按租户隔离）
     * @param name 服务名称
     * @param tenantId 租户ID
     * @return  healthcare_service.id
     */
    String selectHealthcareServiceIdByName(@Param("name") String name, @Param("tenantId") String tenantId);

    /**
     * 获取第一个可用的 healthcare_service.id（名称匹配不到时的兜底）
     * @return  healthcare_service.id
     */
    String selectFirstHealthcareServiceId();
}
