package com.zeroone.star.j3.register.service;

import com.baomidou.mybatisplus.extension.service.IService;
import com.zeroone.star.j3.register.entity.HealthcareService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.query.j3.register.RegisterQuery;
import com.zeroone.star.project.vo.j3.register.RegisterNameVO;
import com.zeroone.star.project.vo.j3.register.RegisterVO;

import java.util.List;

/**
 * <p>
 * 挂号类型 服务类
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author j3小组
 * @version 1.0.0
 */
public interface IRegisterTypeService extends IService<HealthcareService> {

    /**
     * 分页+关键词查询挂号类型列表
     *
     * @param query 查询条件
     * @return 分页数据
     */
    PageDTO<RegisterVO> queryRegisterList(RegisterQuery query);

    /**
     * 获取挂号类型名称列表（不分页，仅启用的，用于下拉选择号源类型）
     *
     * @return 名称列表（id + name）
     */
    List<RegisterNameVO> queryRegisterNames();
}
