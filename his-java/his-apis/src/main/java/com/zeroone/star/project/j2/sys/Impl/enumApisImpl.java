package com.zeroone.star.project.j2.sys.Impl;

import com.zeroone.star.personcenter.entity.EnumValue;
import com.zeroone.star.personcenter.repository.enumRepository;
import com.zeroone.star.personcenter.repository.enumTypeRepository;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j2.sys.enumdto.EnumDTO;
import com.zeroone.star.project.dto.j2.sys.enumdto.EnumDeleteDTO;
import com.zeroone.star.project.j2.sys.enumApis;
import com.zeroone.star.project.query.j2.sys.EnumQuery;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.servers.Server;
import jakarta.annotation.Resource;
import org.springframework.util.StringUtils;

import java.util.ArrayList;
import java.util.List;

@Server
public class enumApisImpl implements enumApis {
    @Resource
    enumTypeRepository eTRepository;

    @Resource
    enumRepository eRepository;

    @Override
    public JsonVO<Integer> deleteEnum(EnumDeleteDTO dto) {
        if(dto!= null){
            return JsonVO.success(dto.getEnumCode());
        }
        return JsonVO.fail(null);
    }

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

    @Override
    public JsonVO<PageDTO<EnumDTO>> QueryAllEnum(EnumQuery query) {

        return null;
    }

    @Override
    public JsonVO<String> deleteEnumType(String enumKey) {
        return null;
    }
}
