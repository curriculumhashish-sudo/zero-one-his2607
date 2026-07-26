package com.zeroone.star.project.j2.sys.Impl;

import com.zeroone.star.personcenter.entity.EnumValue;
import com.zeroone.star.personcenter.repository.enumTypeRepository;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j2.sys.enumdto.EnumDTO;
import com.zeroone.star.project.dto.j2.sys.enumdto.EnumDeleteDTO;
import com.zeroone.star.project.j2.sys.enumApis;
import com.zeroone.star.project.query.j2.sys.EnumQuery;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.servers.Server;
import jakarta.annotation.Resource;
import org.springframework.data.domain.Page;
import org.springframework.data.domain.PageRequest;
import org.springframework.data.mongodb.core.MongoTemplate;
import org.springframework.data.mongodb.core.query.Criteria;
import org.springframework.data.mongodb.core.query.Query;
import org.springframework.util.StringUtils;

import java.util.ArrayList;
import java.util.List;

@Server
public class enumApisImpl implements enumApis {
    @Resource
    MongoTemplate template;

    @Resource
    enumTypeRepository eTRepository;

    /*
     * 删除枚举
     * @param dto 枚举删除对象DTO
     * @return 枚举序号
     */
    @Override
    public JsonVO<Integer> deleteEnum(EnumDeleteDTO dto) {
        if(dto!= null){
            return JsonVO.success(dto.getEnumCode());
        }
        return JsonVO.fail(null);
    }

    /**
     * 获取枚举名称列表
     * @param enumKey 枚举类型编码
     * @return 枚举值名称列表
     */
    @Override
    public JsonVO<List<String>> getEnumNameList(String enumKey) {
        if(StringUtils.hasText(enumKey)){
            List<String> displays = new ArrayList<>();
            List<EnumValue> enumValues = eTRepository.findByEnumKey(enumKey);
            enumValues.forEach(enumValue -> {displays.add(enumValue.getDisplay());});
            return JsonVO.success(displays);
        }
        return JsonVO.fail(null);
    }

    /**
     * 获取枚举列表(条件+分页)
     * @param query 枚举值关键词查询对象
     * @return 枚举值列表
     */
    @Override
    public JsonVO<PageDTO<EnumDTO>> QueryAllEnum(EnumQuery query) {
        //分页请求
        Page<EnumDTO> page = new Page<>(query.getPageIndex(), query.getPageSize());
        //查询条件
        if(StringUtils.hasText(query.getEnumKey())){
            Criteria c1 = Criteria.where("enum_key").is(query.getEnumKey());
            Query q1 = new Query(c1).with(pageRequest);
            List<EnumDTO> enumDTOS = template.find(q1, EnumDTO.class);
            return JsonVO.success(enumDTOS);
        }
        else if(StringUtils.hasText(query.getDisplay())){
            Criteria c2 = Criteria.where("display").is(query.getDisplay());
            Query q2 = new Query(c2).with(pageRequest);
            List<EnumDTO> enumDTOS = template.find(q2, EnumDTO.class);
            return JsonVO.success(enumDTOS);
        }
        else if(StringUtils.hasText(query.getEnumKey()) && StringUtils.hasText(query.getDisplay())){
            Criteria c3 = Criteria.where("enum_key").is(query.getEnumKey()).and("display").is(query.getDisplay());
            Query q3 = new Query(c3).with(pageRequest);
            List<EnumDTO> enumDTOS = template.find(q3, EnumDTO.class);
            return JsonVO.success(enumDTOS);
        }
        return JsonVO.fail(null);
    }

    /**
     * 删除枚举类型
     * @param enumKey 枚举类型唯一键
     * @return 枚举类型唯一键
     */
    @Override
    public JsonVO<String> deleteEnumType(String enumKey) {
        if(StringUtils.hasText(enumKey))
        {
            Criteria c = Criteria.where("enum_type_key").is(enumKey);
            Query query = new Query(c);
            template.remove(query);
            return JsonVO.success("enumKey");
        }
        return JsonVO.fail(null);
    }
}
