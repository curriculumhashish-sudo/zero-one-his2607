package com.zeroone.star.sysmgr.service;

import com.baomidou.mybatisplus.extension.service.IService;
import com.zeroone.star.project.dto.j2.sysmgr.DictItemUpdateDTO;
import com.zeroone.star.sysmgr.entity.TerConcept;

/**
 * <p>
 * 字典项 服务类
 * </p>
 *
 * @author zeroone
 */
public interface ITerConceptService extends IService<TerConcept> {

    /**
     * 修改字典项
     * @param dto 修改参数
     */
    void updateDict(DictItemUpdateDTO dto);

    /**
     * 删除字典项
     * @param id 字典项ID
     */
    void deleteDict(Long id);

}
