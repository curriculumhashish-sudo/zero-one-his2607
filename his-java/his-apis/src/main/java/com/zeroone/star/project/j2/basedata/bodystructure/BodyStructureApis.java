package com.zeroone.star.project.j2.basedata.bodystructure;

import com.zeroone.star.project.dto.j2.basedata.bodystructure.BodyStructureStatusDTO;
import com.zeroone.star.project.dto.j2.basedata.bodystructure.BodyStructureUpdateDTO;
import com.zeroone.star.project.vo.JsonVO;

/**
 * <p>
 * 描述：部位管理接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author 雾止洲
 * @version 1.0.0
 */
public interface BodyStructureApis {

    /**
     * 修改部位。
     *
     * @param dto 修改数据
     * @return 修改结果
     */
    JsonVO<Void> modifyBodyStructure(BodyStructureUpdateDTO dto);

    /**
     * 删除部位。
     *
     * @param id 部位主键
     * @return 删除结果
     */
    JsonVO<Void> removeBodyStructure(String id);

    /**
     * 启用或停用部位。
     *
     * @param dto 状态修改数据
     * @return 修改结果
     */
    JsonVO<Void> modifyBodyStructureStatus(BodyStructureStatusDTO dto);
}
