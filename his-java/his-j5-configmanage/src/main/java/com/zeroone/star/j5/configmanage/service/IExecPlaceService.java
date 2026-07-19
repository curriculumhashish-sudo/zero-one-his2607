package com.zeroone.star.j5.configmanage.service;

import com.baomidou.mybatisplus.extension.service.IService;
import com.zeroone.star.j5.configmanage.dto.ExecPlaceDTO;
import com.zeroone.star.j5.configmanage.entity.ExecPlace;
import com.zeroone.star.project.vo.JsonVO;

/**
 * <p>
 * 执行地点 服务接口
 * </p>
 * @author 阿伟学长
 */
public interface IExecPlaceService extends IService<ExecPlace> {

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
