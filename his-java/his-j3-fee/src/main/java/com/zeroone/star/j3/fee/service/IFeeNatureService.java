package com.zeroone.star.j3.fee.service;

import com.baomidou.mybatisplus.extension.service.IService;
import com.zeroone.star.j3.fee.entity.FeeNature;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j3.fee.FeeNatureDTO;
import com.zeroone.star.project.query.j3.fee.FeeNatureQuery;
import com.zeroone.star.project.vo.j3.fee.FeeNatureVO;
import jakarta.validation.Valid;

import java.util.List;

/**
 * <p>
 * 费用性质 服务类
 * </p>
 * @author 白文儿
 * @version 1.0.0
 */
public interface IFeeNatureService extends IService<FeeNature> {

    /**
     * 分页查询费用性质
     * @param query 查询条件（含分页、关键词）
     * @return 分页结果（含 total、pages、rows）
     */
    PageDTO<FeeNatureVO> listFeeNatures(FeeNatureQuery query);

    /**
     * 修改费用性质
     * @param id  费用性质ID
     * @param dto 费用性质数据
     * @throws RuntimeException 业务失败时抛出
     */
    void updateFeeNature(String id, FeeNatureDTO dto);

    /**
     * 停/启用费用性质
     * @param id        费用性质ID
     * @param isEnabled 是否启用 1启用 0禁用
     * @throws RuntimeException 业务失败时抛出
     */
    void updateStatus(String id, Integer isEnabled);

    /**
     * 查询费用性质名称
     * @return 费用性质的编码和名字
     */
    List<FeeNatureVO> getNameList();


    /**
     * 添加费用性质
     */
    void add(@Valid FeeNatureDTO dto);

    /**
     * 删除费用性质
     */
    void delete(String id);
}
