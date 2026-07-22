package com.zeroone.star.project.j2.sys;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j2.sys.RentDTO;
import com.zeroone.star.project.dto.j2.sys.RentNameDTO;
import com.zeroone.star.project.query.j2.sys.RentNameQuery;
import com.zeroone.star.project.query.j2.sys.RentQuery;
import com.zeroone.star.project.vo.JsonVO;
//import io.swagger.v3.core.util.Json;

public interface RentApis {


    /**
     * 获取租户名称
     * @param rentNameQuery 查询参数
     * @return 租户名称列表
     */
    JsonVO<PageDTO<RentNameDTO>> queryName(RentNameQuery rentNameQuery);

    /**
     * 获取租户列表
     * @param rentQuery 查询参数
     * @return 租户信息分页结果
     */
    JsonVO<PageDTO<RentDTO>> queryAll(RentQuery rentQuery);

}
