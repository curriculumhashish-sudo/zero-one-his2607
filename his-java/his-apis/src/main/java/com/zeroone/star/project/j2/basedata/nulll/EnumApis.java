package com.zeroone.star.project.j2.basedata.nulll;


import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j2.basedata.enumdto.*;
import com.zeroone.star.project.query.j2.basedata.EnumListQuery;
import com.zeroone.star.project.query.j2.basedata.EnumQuery;
import com.zeroone.star.project.query.j2.basedata.EnumTypeQuery;
import com.zeroone.star.project.vo.JsonVO;

import java.util.List;

// TODO: 建议将其移动到enums包下，统一管理格式
public interface EnumApis {

    /**
     * 获取枚举名称列表
      * @param query 枚举查询对象DTO
      * @return 枚举查询对象DTO
     */
    JsonVO<List<String>> getEnumTypeName(EnumListQuery query);

    /**
     * 获取枚举列表(条件+分页)
     * @param query 枚举查询对象DTO
     * @return 枚举类型分页查询对象DTO
     */
    JsonVO<PageDTO<EnumDTO>> queryAllEnum(EnumQuery query);



    /**
     * 根据枚举类型查询枚举列表
      * @param enumTypeId 枚举类型ID
     * @return 枚举类型分页查询对象列表DTO
     */
    JsonVO<List<EnumDTO>> listEnumsByEnumType(String enumTypeId);

    /**
     * 添加枚举
     * @param data 添加参数
     * @return 添加结果
     */
    JsonVO<String> addEnum(EnumAddDTO data);

    /**
     * 修改枚举
     * @param data 修改参数
     * @return 修改结果
     */
    JsonVO<String> modifyEnum(EnumModifyDTO data);

    /**
     * 删除枚举
     * @param id 枚举ID
     * @return 删除结果
     */
    JsonVO<String> deleteEnum(String id);

    /**
     * 分页查询枚举类型
     * @param query 查询参数
     * @return 查询结果
     */
    JsonVO<PageDTO<EnumTypeDTO>> queryAllEnumType(EnumTypeQuery query);

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

    /**
     * 删除枚举类型
     * @param dto 枚举删除对象DTO
     * @return 枚举类型id
     */
    JsonVO<String> deleteEnumType(EnumDeleteDTO dto);
}
