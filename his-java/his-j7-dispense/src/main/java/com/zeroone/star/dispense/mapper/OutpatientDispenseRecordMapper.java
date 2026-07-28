package com.zeroone.star.dispense.mapper;

import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.project.dto.j7.dispense.DispenseFlatDTO;
import com.zeroone.star.project.query.j7.dispense.DispenseQuery;
import org.apache.ibatis.annotations.Param;

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
public interface OutpatientDispenseRecordMapper {

    Page<DispenseFlatDTO> selectOutpatientDispensePage(
            Page<DispenseFlatDTO> page,
            @Param("query") DispenseQuery query
    );

    List<DispenseFlatDTO> selectOutpatientDispenseFlatList(
            @Param("query") DispenseQuery query,
            @Param("limit") int limit
    );
}
