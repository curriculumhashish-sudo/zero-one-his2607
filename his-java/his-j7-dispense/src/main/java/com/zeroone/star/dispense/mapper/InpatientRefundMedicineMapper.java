package com.zeroone.star.dispense.mapper;

import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.project.query.j7.dispense.InpatientRefundDetailQuery;
import com.zeroone.star.project.query.j7.dispense.InpatientRefundPatientQuery;
import com.zeroone.star.project.vo.j7.dispense.InpatientRefundDetailVO;
import com.zeroone.star.project.vo.j7.dispense.InpatientRefundPatientVO;
import org.apache.ibatis.annotations.Param;

import java.util.List;

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

    /**
     * 查询指定就诊下的退药明细
     */
    List<InpatientRefundDetailVO> selectRefundDetailList(
            @Param("query") InpatientRefundDetailQuery query
    );
}
