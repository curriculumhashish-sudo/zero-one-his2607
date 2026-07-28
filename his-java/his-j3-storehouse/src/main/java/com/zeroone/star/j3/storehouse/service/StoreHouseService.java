package com.zeroone.star.j3.storehouse.service;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j3.storehouse.StoreHouseAddDTO;
import com.zeroone.star.project.dto.j3.storehouse.StoreHouseUpdateDTO;
import com.zeroone.star.project.query.j3.storehouse.StoreHouseQuery;
import com.zeroone.star.project.vo.j3.storehouse.DepartmentSimpleVO;
import com.zeroone.star.project.vo.j3.storehouse.StoreHouseVO;

import java.util.List;

/**
 * 库房药房服务接口
 * @author mie
 * @version 1.0.0
 */
public interface StoreHouseService {

    /**
     * 获取库房药房名称（支持类型筛选）
     */
    List<StoreHouseVO> getStoreHouseNames(String formEnum);

    /**
     * 获取库房药房列表（条件+分页）
     */
    PageDTO<StoreHouseVO> list(StoreHouseQuery query);

    /**
     * 添加库房药房
     */
    void add(StoreHouseAddDTO dto);

    /**
     * 修改库房药房
     */
    void update(StoreHouseUpdateDTO dto);

    /**
     * 删除库房药房
     */
    void delete(Long id);

    /**
     * 启/停用库房药房
     */
    void toggleEnabled(Long id, Byte isEnabled);

    /**
     * 获取上级科室列表（科室药房选择上级用）
     */
    List<DepartmentSimpleVO> listParentDepartments();
}
