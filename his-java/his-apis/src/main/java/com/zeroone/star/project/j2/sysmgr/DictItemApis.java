package com.zeroone.star.project.j2.sysmgr;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j2.basedata.DictItemUpdateDTO;
import com.zeroone.star.project.dto.j2.sysmgr.dictiemdto.DictItemAddDTO;
import com.zeroone.star.project.dto.j2.sysmgr.dictiemdto.DictItemDTO;
import com.zeroone.star.project.query.j2.sysmgr.DictItemQuery;
import com.zeroone.star.project.vo.JsonVO;

import java.util.List;

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
     * 获取字典名称列表
     * @return 字典名称列表
     */
    JsonVO<List<String>> queryDictNameList();
    /**
     * 获取字典列表(条件+分页)
     * @param query 字典查询条件
     * @return 字典分页结果
     */
    JsonVO<PageDTO<DictItemDTO>> queryAll(DictItemQuery query);
    /**
     * 新增字典
     * @param addDTO 字典添加信息
     * @return 添加结果
     */
    JsonVO<String> addDictItem(DictItemAddDTO addDTO);

    /**
     *字典更新
     * @param updateDTO 字典更新的对象
     * @return 更行结果
     */
    JsonVO<String> updateDict(DictItemUpdateDTO updateDTO);

    /**
     * 字典删除
     * @param id 字典编码的唯一标识
     * @return 删除结果
     */
    JsonVO<String> deleteDict(Long id);

}
