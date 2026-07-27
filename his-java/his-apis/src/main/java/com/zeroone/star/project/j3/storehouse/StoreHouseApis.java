package com.zeroone.star.project.j3.storehouse;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j3.storehouse.StoreHouseDTO;
import com.zeroone.star.project.query.j3.storehouse.StoreHouseQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j3.storehouse.StoreHouseVO;

import java.util.List;

/**
 * 描述：库房药房接口
 * @author mie
 * @version 1.0.0
 */
public interface StoreHouseApis {

    /**
     * 获取库房药房名称（支持类型筛选）
     * @param type 仓库类型ID，传null则返回全部
     * @return 库房药房名称列表
     */
    JsonVO<List<StoreHouseVO>> getStoreHouseNames(Long type);

    /**
     * 获取库房药房列表（条件+分页）
     * @param query 查询条件（关键字）
     * @return 分页数据
     */
    JsonVO<PageDTO<StoreHouseVO>> list(StoreHouseQuery query);

    /**
     * 添加库房药房
     * @param dto 新增数据（不含ID）
     * @return 操作结果
     */
    JsonVO<String> add(StoreHouseDTO dto);

    /**
     * 修改库房药房
     * @param dto 编辑数据（含ID）
     * @return 操作结果
     */
    JsonVO<String> update(StoreHouseDTO dto);

    /**
     * 删除库房药房
     * @param id 库房药房ID
     * @return 操作结果
     */
    JsonVO<String> delete(Long id);

    /**
     * 启/停用库房药房
     * @param id 库房药房ID
     * @param isEnabled 1启用 0停用
     * @return 操作结果
     */
    JsonVO<String> toggleEnabled(Long id, Byte isEnabled);
}
