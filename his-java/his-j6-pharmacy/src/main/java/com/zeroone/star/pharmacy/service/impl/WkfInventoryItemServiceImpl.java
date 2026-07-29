package com.zeroone.star.pharmacy.service.impl;

import com.zeroone.star.pharmacy.entity.WkfInventoryItem;
import com.zeroone.star.pharmacy.mapper.WkfInventoryItemMapper;
import com.zeroone.star.pharmacy.service.IWkfInventoryItemService;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.project.dto.j6.pharmacy.inventory.InventoryStatusDTO;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

/**
 * <p>
 * 工作流 inventory item 服务实现类
 * </p>
 *
 * @author leisurely
 * @since 2026-07-24
 */
@Service
public class WkfInventoryItemServiceImpl extends ServiceImpl<WkfInventoryItemMapper, WkfInventoryItem> implements IWkfInventoryItemService {

    @Autowired
    WkfInventoryItemMapper wkfInventoryItemMapper;


    @Override
    public Integer resumeInventoryItems(InventoryStatusDTO inventoryStatusDTO) {
        // TODO: 写一个加锁版本的

        // 修改库存商品状态,并且返回修改成功行数
        return wkfInventoryItemMapper.updateInventoryStatus(inventoryStatusDTO.getInventoryItemIds());
    }
}
