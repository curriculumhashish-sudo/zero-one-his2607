package com.zeroone.star.project.j2.sysmgr;

import com.zeroone.star.project.dto.j2.sysmgr.DictItemUpdateDTO;
import com.zeroone.star.project.vo.JsonVO;

public interface DictItemApis {

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
