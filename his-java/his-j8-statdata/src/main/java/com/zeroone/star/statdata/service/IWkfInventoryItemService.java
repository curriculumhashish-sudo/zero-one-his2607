package com.zeroone.star.statdata.service;

import com.zeroone.star.project.query.j8.statdata.HourTrendQuery;
import com.zeroone.star.project.vo.j8.statdata.ExpireWarningVO;
import com.zeroone.star.statdata.entity.WkfInventoryItem;
import com.baomidou.mybatisplus.extension.service.IService;

/**
 * <p>
 * 库存有效期服务接口
 * </p>
 *
 * @author highxin
 * @since 2026-07-24
 */
public interface IWkfInventoryItemService extends IService<WkfInventoryItem> {
    /**
     * 获取过期预警饼图数据
     * @param query 查询参数
     * @return 过期预警饼图数据
     */
    ExpireWarningVO getExpireWarningData(HourTrendQuery query);
}