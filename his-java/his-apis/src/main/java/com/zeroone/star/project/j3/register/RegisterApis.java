package com.zeroone.star.project.j3.register;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j3.register.RegisterDTO;
import com.zeroone.star.project.query.j3.register.RegisterQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j3.register.RegisterVO;

/**
 * <p>
 * 描述：挂号类型管理接口
 * <p>说明：本接口定义了挂号类型（普通号、专家号、特需号等）的
 * 列表查询、新增、修改、删除等前后端交互接口。</p>
 *
 * @author j3小组
 * @version 1.0.0
 */
public interface RegisterApis {

    /**
     * 获取挂号类型列表（条件+分页）
     *
     * @param query 分页查询条件（关键词可对名称、医保编码、id进行模糊查询）
     * @return 分页数据结果
     */
    JsonVO<PageDTO<RegisterVO>> queryRegisterList(RegisterQuery query);

    /**
     * 新增挂号类型
     *
     * @param dto 新增数据
     * @return 新增结果（成功返回受影响行数等）
     */
    JsonVO<Boolean> addRegister(RegisterDTO dto);

    /**
     * 修改挂号类型
     *
     * @param id  待修改的挂号类型唯一标识（路径参数）
     * @param dto 修改数据
     * @return 修改结果
     */
    JsonVO<Boolean> modifyRegister(Long id, RegisterDTO dto);

    /**
     * 删除挂号类型
     *
     * @param id 待删除的挂号类型唯一标识
     * @return 删除结果
     */
    JsonVO<Boolean> removeRegister(Long id);
}
