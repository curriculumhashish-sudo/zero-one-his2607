package com.zeroone.star.basedata.service.impl;

import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.basedata.entity.Medicine;
import com.zeroone.star.basedata.mapper.MedicineMapper;
import com.zeroone.star.basedata.service.IMedicineService;
import org.springframework.stereotype.Service;

/**
 * <p>
 * 描述：药品管理服务实现
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@Service
public class MedicineServiceImpl extends ServiceImpl<MedicineMapper, Medicine> implements IMedicineService {
}
