package com.zeroone.star.project.j6.pharmacy;

import com.zeroone.star.project.dto.j6.pharmacy.InventoryStatusDTO;
import com.zeroone.star.project.query.j6.pharmacy.InventoryDetailQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j6.pharmacy.ExcelExportVO;
import com.zeroone.star.project.vo.j6.pharmacy.InventoryDetailVO;

public interface PharmacyApis {

    /**
     * 获取库存明细列表(分页+条件)
     * 请求路径 /pageInventoryDetailCurrent
     * 请求方式 post
     * @param inventoryDetailQueryDTO 分页条件
     * @return 返回查询到的所有数据
     */
    JsonVO<InventoryDetailVO> pageInventoryDetailCurrent(InventoryDetailQuery inventoryDetailQueryDTO);


    /**
     * 停供/取消停供
     * 请求路径 /resumeInventoryItems
     * 请求方式 post
     * @param inventoryStatusDTO 需要停供/取消停供 的所有商品id
     * @return 成功修改了多少条数据
     */
    JsonVO<Integer> updateStatus(InventoryStatusDTO inventoryStatusDTO);


    /**
     * 导出数据
     * 请求路径 /exportInventoryDetailCurrent
     * 请求方式 post
     * @param inventoryDetailQueryDTO 条件 (根据条件导出excel)
     * @return 导出的数据
     */
    JsonVO<ExcelExportVO> exportInventoryDetailCurrent(InventoryDetailQuery inventoryDetailQueryDTO);
}
