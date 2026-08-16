package com.zeroone.star.dispense.service;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j7.dispense.OutpatientDispenseRecordDTO;
import com.zeroone.star.project.query.j7.dispense.DispenseQuery;
import com.zeroone.star.project.vo.j7.dispense.DispenseGroupVO;

import java.util.List;

/**
 * <p>
 * 描述：门诊发退药记录服务实现
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 再睡一夏
 * @version 1.0.0
 */

public interface IOutpatientDispenseRecordService {

    /**
     * 分页查询发退药记录（按处方分组）
     */
    PageDTO<DispenseGroupVO> queryPage(DispenseQuery query);

    /**
     * 获取平铺格式导出数据
     */
    List<OutpatientDispenseRecordDTO> listForExport(DispenseQuery query);
}
