package com.zeroone.star.basedata.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.basedata.entity.medicine.MedMedicationDefinitionDO;
import com.zeroone.star.project.query.j4.medicine.MedicinePageQuery;
import com.zeroone.star.project.query.j4.medicine.MedicineSelectPageQuery;
import com.zeroone.star.project.vo.j4.medicine.MedicineListVO;
import com.zeroone.star.project.vo.j4.medicine.MedicineSelectVO;
import org.apache.ibatis.annotations.Param;

/**
 * <p>
 * 描述：药品定义数据访问接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
public interface MedMedicationDefinitionMapper
        extends BaseMapper<MedMedicationDefinitionDO> {

    Page<MedicineListVO> selectMedicinePage(
            Page<MedicineListVO> page,
            @Param("query") MedicinePageQuery query,
            @Param("tenantId") String tenantId
    );

    /**
     * 条件分页查询可选择的药品
     *
     * @param page 分页对象
     * @param query 查询条件
     * @param locationId 当前药房或库房标识
     * @return 可选择药品分页数据
     */
    Page<MedicineSelectVO> selectMedicineSelectPage(
            Page<MedicineSelectVO> page,
            @Param("query") MedicineSelectPageQuery query,
            @Param("locationId") String locationId,
            @Param("tenantId") String tenantId
    );
}
