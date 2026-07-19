package com.zeroone.star.project.j2.sys;


import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j2.sys.EnumDTO;
import com.zeroone.star.project.query.j2.sys.EnumListQuery;
import com.zeroone.star.project.query.j2.sys.EnumQuery;
import com.zeroone.star.project.vo.JsonVO;

import java.util.List;


public interface enumApis {
    /**
     * 删除枚举类型
     * @param query 枚举查询对象DTO
     * @return 枚举类型id
     */
    JsonVO<String> deleteEnumType(EnumListQuery query);

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
