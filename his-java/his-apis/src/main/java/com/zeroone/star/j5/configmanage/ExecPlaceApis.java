package com.zeroone.star.j5.configmanage;

import com.zeroone.star.j5.configmanage.dto.ExecPlaceDTO;
import com.zeroone.star.project.vo.JsonVO;

/**
 * <p>
 * 描述：执行地点管理接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 阿伟学长
 * @version 1.0.0
 */
public interface ExecPlaceApis {

    /**
     * 新增执行地点
     * @param execPlaceDTO 执行地点数据
     * @return 操作结果
     */
    JsonVO<String> add(ExecPlaceDTO execPlaceDTO);

    /**
     * 修改执行地点
     * @param execPlaceDTO 执行地点数据
     * @return 操作结果
     */
    JsonVO<String> update(ExecPlaceDTO execPlaceDTO);

    /**
     * 删除执行地点
     * @param id 执行地点ID
     * @return 操作结果
     */
    JsonVO<String> delete(Long id);
}
