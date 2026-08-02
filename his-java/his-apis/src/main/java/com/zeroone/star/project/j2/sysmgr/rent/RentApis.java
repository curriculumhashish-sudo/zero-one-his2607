package com.zeroone.star.project.j2.sysmgr.rent;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j2.sysmgr.rentdto.RentNameDTO;
import com.zeroone.star.project.dto.j2.sysmgr.rentdto.RentDTO2;
import com.zeroone.star.project.dto.j2.sysmgr.rentdto.RentDTO;
import com.zeroone.star.project.dto.j2.sysmgr.rentdto.RentAddDTO;
import com.zeroone.star.project.query.j2.sysmgr.rent2.RentNameQuery;
import com.zeroone.star.project.query.j2.sysmgr.rent2.RentQuery;
import com.zeroone.star.project.vo.JsonVO;
/**
 * <p>
 * 描述：租户管理接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 阿伟学长
 * @version 1.0.0
 */

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
    JsonVO<PageDTO<RentDTO2>> queryAll(RentQuery rentQuery);


    /**
     * 添加租户信息
     * @param addDTO 租户添加信息
     * @return 添加结果
     */
    JsonVO<String> addRent(RentAddDTO addDTO);
    /**
     * 修改租户信息
      * @param updateDTO 租户修改信息
     * @return 修改结果
     */
    JsonVO<String> updateRent(String id,RentDTO updateDTO );
    /**
     * 删除租户信息
     * @param id 租户ID
     * @return 删除结果
     */
    JsonVO<String> deleteRent(String id);
}
