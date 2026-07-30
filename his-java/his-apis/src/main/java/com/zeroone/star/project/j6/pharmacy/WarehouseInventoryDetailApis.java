package com.zeroone.star.project.j6.pharmacy;

import com.baomidou.mybatisplus.core.metadata.IPage;
import com.zeroone.star.project.dto.j6.pharmacy.inventory.InventoryStatusDTO;
import com.zeroone.star.project.query.j6.pharmacy.inventory.InventoryDetailQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j6.pharmacy.inventory.InventoryDetailVO;
import jakarta.servlet.http.HttpServletResponse;
import org.springframework.web.bind.annotation.RequestBody;
import java.io.IOException;

public interface WarehouseInventoryDetailApis {
    /**
     * 获取库存明细列表(分页+条件)
     * 请求路径 /pageInventoryDetailCurrent
     * 请求方式 post
     * @param inventoryDetailQueryDTO 分页条件
     * @return 返回查询到的所有数据
     */
    JsonVO<IPage<InventoryDetailVO>> pageInventoryDetailCurrent(InventoryDetailQuery inventoryDetailQueryDTO);


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
     * @return 一个编码为base64的excel文件流
     */
    void exportInventoryDetailCurrent(@RequestBody InventoryDetailQuery inventoryDetailQueryDTO,
                                                       HttpServletResponse response) throws IOException;

}
