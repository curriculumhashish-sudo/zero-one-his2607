package com.zeroone.star.project.j2.sys;

import com.zeroone.star.project.dto.j2.sys.EnumTypeDeleteDTO;
import com.zeroone.star.project.vo.JsonVO;

public interface enumTypeApis {
    /**
     * 删除枚举类型
     * @param dto
     * @return 枚举类型唯一键
     */
    JsonVO<String> deleteEnumType(EnumTypeDeleteDTO dto);
}
