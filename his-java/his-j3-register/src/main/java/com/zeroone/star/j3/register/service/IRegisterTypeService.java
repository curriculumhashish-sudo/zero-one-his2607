package com.zeroone.star.j3.register.service;

import com.baomidou.mybatisplus.extension.service.IService;
import com.zeroone.star.j3.register.entity.RegisterType;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j3.register.RegisterDTO;
import com.zeroone.star.project.query.j3.register.RegisterQuery;
import com.zeroone.star.project.vo.j3.register.RegisterVO;


/**
 * <p>
 * 描述：挂号类型服务接口
 * </p>
 *
 * @author j3小组
 * @version 1.0.0
 */
public interface IRegisterTypeService extends IService<RegisterType> {

    /**
     * 分页查询挂号类型列表
     *
     * @param query 分页查询条件（关键词可对名称、医保编码、id进行模糊查询）
     * @return 分页数据
     */
    PageDTO<RegisterVO> queryRegisterList(RegisterQuery query);

    /**
     * 新增挂号类型
     *
     * @param dto 新增数据
     * @return 是否成功
     */
    boolean addRegister(RegisterDTO dto);

    /**
     * 修改挂号类型
     *
     * @param id  待修改的挂号类型唯一标识
     * @param dto 修改数据
     * @return 是否成功
     */
    boolean modifyRegister(Long id, RegisterDTO dto);

    /**
     * 删除挂号类型
     *
     * @param id 挂号类型唯一标识
     * @return 是否成功
     */
    boolean removeRegister(Long id);
}
