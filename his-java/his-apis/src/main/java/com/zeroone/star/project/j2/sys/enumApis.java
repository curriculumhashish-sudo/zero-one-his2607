package com.zeroone.star.project.j2.sys;


import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j2.sys.enumdto.EnumDTO;
import com.zeroone.star.project.dto.j2.sys.enumdto.EnumDeleteDTO;
import com.zeroone.star.project.query.j2.sys.EnumQuery;
import com.zeroone.star.project.vo.JsonVO;

import java.util.List;


public interface enumApis {
    /**
     * 删除枚举
     * @param dto 枚举删除对象DTO
     * @return 枚举序号
     */
    JsonVO<Integer> deleteEnum(EnumDeleteDTO dto);

    /**
     * 获取枚举名称列表
      * @param dto 枚举值名字列表查询对象
      * @return 枚举值名称列表
     */
    JsonVO<List<String>> getEnumNameList(EnumDTO dto);

    /**
     * 获取枚举列表(条件+分页)
     * @param query 枚举值关键词查询对象
     * @return 枚举值列表
     */
    JsonVO<PageDTO<EnumDTO>> QueryAllEnum(EnumQuery query);

    /**
     * 删除枚举类型
     * @param enumKey 枚举类型唯一键
     * @return 枚举类型唯一键
     */
    JsonVO<String> deleteEnumType(String enumKey);

}
