package com.zeroone.star.basedata.service;

import com.baomidou.mybatisplus.extension.service.IService;
import com.zeroone.star.basedata.entity.medicine.MedMedicationDefinitionDO;
import com.zeroone.star.project.dto.j4.medicine.MedicineChangeStatusDTO;
import com.zeroone.star.project.dto.j4.medicine.MedicineDeleteDTO;
import com.zeroone.star.project.dto.j4.medicine.MedicineDetailDTO;
import com.zeroone.star.project.query.j4.medicine.MedicinePageQuery;
import com.zeroone.star.project.query.j4.medicine.MedicineSelectPageQuery;
import com.zeroone.star.project.vo.j4.medicine.MedicineAddVO;
import com.zeroone.star.project.vo.j4.medicine.MedicineListVO;
import com.zeroone.star.project.vo.j4.medicine.MedicineSelectVO;

/**
 * <p>
 * 描述：药品管理服务接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
public interface IMedicineService extends IService<MedMedicationDefinitionDO> {

    /**
     * 条件分页查询药品列表
     * @param query 查询条件和分页参数
     * @return 药品分页列表
     */
    PageDTO<MedicineListVO> queryMedicinePage(MedicinePageQuery query);

    /**
     * 条件分页查询可选择的药品
     *
     * @param query 查询条件和分页参数
     * @return 可选择药品分页列表
     */
    PageDTO<MedicineSelectVO> queryMedicineSelectPage(
            MedicineSelectPageQuery query
    );

    /**
     * 根据ID查询药品详情
     * @param id 药品ID
     * @return 药品详情
     */
    MedicineDetailDTO queryMedicineDetailById(String id);

    /**
     * 更新单条药品记录
     * @param dto 药品更新数据
     * @return 受影响的记录数
     */
    int updateMedicine(MedicineDetailDTO dto);

    /**
     * 批量删除药品记录
     * @param dto 待删除的药品ID集合
     * @return 成功删除的记录数
     */
    int deleteMedicine(MedicineDeleteDTO dto);

    /**
     * 批量修改药品启停状态
     * @param status 药品状态，1表示启用，0表示停用
     * @param dto 待修改状态的药品ID集合
     * @return 成功修改的记录数
     */
    int updateChangeMedicineStatus(int status, MedicineChangeStatusDTO dto);

    /**
     * 新增药品及其价格信息
     * @param dto 新增药品数据
     * @return 新增后的药品信息
     */
    MedicineAddVO addMedicine(MedicineAddDTO dto);
}
