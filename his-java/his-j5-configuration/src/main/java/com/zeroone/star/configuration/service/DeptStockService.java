package com.zeroone.star.configuration.service;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j5.deptstock.DeptStockSaveDTO;
import com.zeroone.star.project.query.j5.deptstock.DeptStockQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j5.deptstock.DeptStockVO;

/**
 * 配置管理-取药科室服务接口
 */
public interface DeptStockService {

    /**
     * 条件+分页查询取药科室项目
     */
    JsonVO<PageDTO<DeptStockVO>> listDeptStock(DeptStockQuery query);

    /**
     * 批量保存取药科室项目
     */
    JsonVO<String> saveDeptStock(DeptStockSaveDTO dto);
}
