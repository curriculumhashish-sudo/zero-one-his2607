package com.zeroone.star.project.j2.basedata;


import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j2.basedata.enumdto.EnumDTO;
import com.zeroone.star.project.dto.j2.basedata.enumdto.EnumDeleteDTO;
import com.zeroone.star.project.query.j2.basedata.EnumListQuery;
import com.zeroone.star.project.query.j2.basedata.EnumQuery;
import com.zeroone.star.project.vo.JsonVO;

import java.util.List;


public interface enumApis {
    /**
     * 删除枚举类型
     * @param dto 枚举删除对象DTO
     * @return 枚举类型id
     */
    JsonVO<String> deleteEnumType(EnumDeleteDTO dto);

    /**
     * 获取枚举名称列表
      * @param query 枚举查询对象DTO
      * @return 枚举查询对象DTO
     */
    JsonVO<List<String>> getEnumTypeName(EnumListQuery query);

    /**
     * 获取枚举列表(条件+分页)
     * @param query 枚举查询对象DTO
     * @return
     */
    JsonVO<PageDTO<EnumDTO>> QueryAllEnum(EnumQuery query);


}
