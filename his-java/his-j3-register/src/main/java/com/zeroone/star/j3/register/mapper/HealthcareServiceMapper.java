package com.zeroone.star.j3.register.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.baomidou.mybatisplus.core.metadata.IPage;
import com.zeroone.star.j3.register.entity.HealthcareService;
import com.zeroone.star.project.vo.j3.register.RegisterVO;
import org.apache.ibatis.annotations.Param;

/**
 * <p>
 * 医疗服务表 Mapper 接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author j3小组
 * @version 1.0.0
 */
public interface HealthcareServiceMapper extends BaseMapper<HealthcareService> {

    /**
     * 分页查询挂号类型（联查收费定义取金额）
     *
     * @param page    分页对象
     * @param keyword 关键词（名称/医保编码/id 模糊）
     * @return 分页结果
     */
    IPage<RegisterVO> selectRegisterPage(@Param("page") IPage<RegisterVO> page, @Param("keyword") String keyword);
}
