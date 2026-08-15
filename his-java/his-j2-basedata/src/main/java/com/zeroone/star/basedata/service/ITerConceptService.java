package com.zeroone.star.basedata.service;

import com.baomidou.mybatisplus.extension.service.IService;
import com.zeroone.star.basedata.entity.TerConcept;
import com.zeroone.star.project.dto.j2.basedata.dictionarydto.DictItemUpdateDTO;


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
