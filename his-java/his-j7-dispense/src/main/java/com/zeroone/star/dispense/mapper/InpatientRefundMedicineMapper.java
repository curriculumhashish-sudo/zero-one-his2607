package com.zeroone.star.dispense.mapper;

import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.project.query.j7.dispense.InpatientRefundPatientQuery;
import com.zeroone.star.project.vo.j7.dispense.InpatientRefundPatientVO;
import org.apache.ibatis.annotations.Param;

/**
 * 住院退药 Mapper
 *
 * @author jacobzp
 * @since 2026-07-26
 */
public interface InpatientRefundMedicineMapper {

    /**
     * 分页查询住院退药患者队列（按就诊聚合）
     */
    Page<InpatientRefundPatientVO> selectPatientQueuePage(
            Page<InpatientRefundPatientVO> page,
            @Param("query") InpatientRefundPatientQuery query
    );
}
