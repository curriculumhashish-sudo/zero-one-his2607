package com.zeroone.star.project.login.j2.basic;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j2.basic.EnumTypeAddDTO;
import com.zeroone.star.project.dto.j2.basic.EnumTypeDTO;
import com.zeroone.star.project.query.j2.basic.EnumTypeQuery;
import com.zeroone.star.project.vo.JsonVO;

public interface EnumApis {

    /**
     * 分页查询
     * @param query 查询参数
     * @return 查询结果
     */
    JsonVO<PageDTO<EnumTypeDTO>> queryAll(EnumTypeQuery query);

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
    JsonVO<String> modifyEnumType(EnumTypeAddDTO data);
}
