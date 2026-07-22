package com.zeroone.star.project.j2.sys;

import com.zeroone.star.project.vo.JsonVO;

public interface enumTypeApis {
    /**
     * 删除枚举类型
      * @param enumKey 枚举类型唯一键
     * @return 枚举类型唯一键
     */
    JsonVO<String> deleteEnumType(String enumKey);
}
