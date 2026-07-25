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
     */
    IPage<RegisterType> selectRegisterTypePage(IPage<RegisterType> page,
                                                @Param("keyword") String keyword);
}
