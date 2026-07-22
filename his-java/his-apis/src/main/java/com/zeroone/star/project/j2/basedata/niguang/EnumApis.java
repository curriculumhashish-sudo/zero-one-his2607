package com.zeroone.star.project.j2.basedata.niguang;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j2.basedata.enumdto.EnumTypeAddDTO;
import com.zeroone.star.project.dto.j2.basedata.enumdto.EnumTypeModifyDTO;
import com.zeroone.star.project.query.j2.basedata.EnumTypeQuery;
import com.zeroone.star.project.vo.JsonVO;


// TODO: 已整合到null的类中，请删除
public interface EnumApis {

    /**
     * 分页查询
     *
     * @param query 查询参数
     * @return 查询结果
     */
    JsonVO<PageDTO<com.zeroone.star.project.dto.j2.basedata.enumdto.EnumTypeDTO>> queryAllEnumType(EnumTypeQuery query);

    /**
     * 添加枚举类型
     * @param data 添加参数
     * @return 添加结果
     */
    JsonVO<String> addEnumType(EnumTypeAddDTO data);

    /**
     * 修改枚举类型
     * @param data 修改参数
     * @return 修改结果
     */
    JsonVO<String> modifyEnumType(EnumTypeModifyDTO data);

}
