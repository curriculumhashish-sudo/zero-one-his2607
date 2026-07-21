package com.zeroone.star.project.j2.sysmgr;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j2.sysmgr.dictiemdto.DictItemAddDTO;
import com.zeroone.star.project.dto.j2.sysmgr.dictiemdto.DictItemDTO;
import com.zeroone.star.project.query.j2.sysmgr.DictItemQuery;
import com.zeroone.star.project.vo.JsonVO;

/**
 * <p>
 * 描述：字典管理接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 阿伟学长
 * @version 1.0.0
 */

public interface DictItemApis {
    /**
     * 分页查询字典信息
     * @param query 字典查询条件
     * @return 字典信息分页结果
     */
    JsonVO<PageDTO<DictItemDTO>> queryAll(DictItemQuery query);
    /**
     * 添加字典信息
     * @param addDTO 字典添加信息
     * @return 添加结果
     */
    JsonVO<String> addDictItem(DictItemAddDTO addDTO);
    /**
     * 修改字典信息
     * @param updateDTO 字典修改信息
     * @return 修改结果
     */
    JsonVO<String> updateDictItem(DictItemDTO updateDTO );
    /**
     * 删除字典信息
     * @param id 字典ID
     * @return 删除结果
     */
    JsonVO<String> deleteDictItem(String id);
}
