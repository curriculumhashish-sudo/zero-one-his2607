package com.zeroone.star.dispense.mapper;

import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.project.dto.j7.dispense.InpatientDispenseRecordDTO;
import com.zeroone.star.project.query.j7.dispense.InpatientDispenseRecordQuery;
import org.apache.ibatis.annotations.Param;

import java.util.List;

public interface InpatientDispenseRecordMapper {

    Page<InpatientDispenseRecordDTO> selectInpatientDispensePage(
            Page<InpatientDispenseRecordDTO> page,
            @Param("query") InpatientDispenseRecordQuery query
    );

    List<InpatientDispenseRecordDTO> selectInpatientDispenseList(
            @Param("query") InpatientDispenseRecordQuery query,
            @Param("limit") int limit
    );
}
