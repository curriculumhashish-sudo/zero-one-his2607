package com.zeroone.star.pharmacy.service;

import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.project.query.j6.pharmacy.inventory.InventoryDetailQuery;
import com.zeroone.star.project.vo.j6.pharmacy.inventory.ExcelExportVO;
import com.zeroone.star.project.vo.j6.pharmacy.inventory.InventoryDetailVO;


public interface InventoryDetailService {

    // 条件分页查询
    Page<InventoryDetailVO> pageInventoryDetailCurrent(InventoryDetailQuery inventoryDetailQueryDTO);

    // 导出excel
    ExcelExportVO exportToExcel(InventoryDetailQuery inventoryDetailQueryDTO);
}
