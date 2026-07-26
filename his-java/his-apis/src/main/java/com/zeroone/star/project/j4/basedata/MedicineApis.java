package com.zeroone.star.project.j4.basedata;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j4.medicine.*;
import com.zeroone.star.project.query.j4.medicine.MedicinePageQuery;
import com.zeroone.star.project.query.j4.medicine.MedicineSelectPageQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j4.medicine.MedicineAddVO;
import com.zeroone.star.project.vo.j4.medicine.MedicineDetailVO;
import com.zeroone.star.project.vo.j4.medicine.MedicineListVO;
import com.zeroone.star.project.vo.j4.medicine.MedicineSelectVO;

/**
 * <p>
 * 描述：药品管理 API 接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
public interface MedicineApis {
    /**
     * 条件分页查询药品列表。
     *
     * @param query 查询条件和分页参数
     * @return 药品分页列表
     */
    JsonVO<PageDTO<MedicineListVO>> queryMedicineList(MedicinePageQuery query);

    /**
     * 条件分页查询可选择的药品列表
     *
     * @param query 查询条件和分页参数
     * @return 可选择药品分页列表
     */
    JsonVO<PageDTO<MedicineSelectVO>> queryMedicineSelectList(
            MedicineSelectPageQuery query
    );

    /**
     * <p>
     * 描述：获取药品详情接口
     * </p>
     *
     * @author xxm
     * @version 1.0.0
     * @param id 药品id
     * @return 药品详情VO
     */
    JsonVO<MedicineDetailVO> queryMedicineDetail(String id);
    /**
    * <p>
    * 描述：修改药品接口
    * </p>
    *
    * @author xxm
    * @version 1.0.0
    * @param dto 药品更新dto
    * @return 更新结果
    */
    JsonVO<String> updateMedicine(MedicineDetailDTO dto);

    /**
     * <p>
     * 描述：删除药品接口（批量）
     * </p>
     *
     * @param dto 药品删除dto
     * @return 药品删除结果dto 包含删除成功药品ids
     * @author xxm
     * @version 1.0.0
     */
    JsonVO<MedicineDeleteResultDTO> deleteMedicine(MedicineDeleteDTO dto);

    /**
    * <p>
    * 描述：启/停用药品接口（批量）
    * </p>
    *
    * @author xxm
    * @version 1.0.0
    * @param dto 药品状态转换DTO
    * @return 药品状态转换VO 包含转换状态成功的药品ids
    */
    JsonVO<Void> updateChangeMedicineStatus(Integer status,MedicineChangeStatusDTO dto);

    /**
     * 新增药品。
     *
     * @param dto 新增药品信息
     * @return 新增成功后的药品ID
     */
    JsonVO<MedicineAddVO> addMedicine(MedicineAddDTO dto);
}
