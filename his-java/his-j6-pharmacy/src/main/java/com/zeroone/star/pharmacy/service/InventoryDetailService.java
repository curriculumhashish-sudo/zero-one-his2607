package com.zeroone.star.pharmacy.service;

import com.baomidou.mybatisplus.core.metadata.IPage;
import com.zeroone.star.project.query.j6.pharmacy.inventory.InventoryDetailQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j6.pharmacy.inventory.InventoryDetailVO;

public interface InventoryDetailService {

    JsonVO<IPage<InventoryDetailVO>> pageInventoryDetailCurrent(InventoryDetailQuery inventoryDetailQueryDTO);
}
