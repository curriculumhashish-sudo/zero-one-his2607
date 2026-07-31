package com.zeroone.star.j3.patient.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.j3.patient.entity.PatientDO;
import com.zeroone.star.project.query.j3.patient.PatientQuery;
import org.apache.ibatis.annotations.Insert;
import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;
import org.apache.ibatis.annotations.Select;
import org.apache.ibatis.annotations.Update;

import java.time.LocalDateTime;

/**
 * 患者档案数据访问层。
 *
 * @author 照夜清
 * @author 波小夏
 */
@Mapper
public interface PatientMapper extends BaseMapper<PatientDO> {

    /**
     * 按原型筛选条件分页查询患者管理列表。
     *
     * @param page  分页参数
     * @param query 查询条件
     * @return 患者分页
     */
    @Select("""
            <script>
            SELECT
                id,
                bus_no,
                name,
                gender_enum,
                birth_date,
                id_card,
                phone,
                status_enum,
                active_flag,
                temp_flag,
                organization_id,
                created_at,
                updated_at,
                tenant_id
            FROM adm_patient
            WHERE is_deleted = 0
              AND tenant_id = #{tenantId}
            <if test="query.keyword != null and query.keyword != ''">
                AND (
                    name LIKE CONCAT('%', #{query.keyword}, '%')
                    OR bus_no LIKE CONCAT('%', #{query.keyword}, '%')
                    OR id_card LIKE CONCAT('%', #{query.keyword}, '%')
                    OR py_str LIKE CONCAT('%', #{query.keyword}, '%')
                    OR wb_str LIKE CONCAT('%', #{query.keyword}, '%')
                )
            </if>
            <if test="query.name != null and query.name != ''">
                AND name LIKE CONCAT('%', #{query.name}, '%')
            </if>
            <if test="query.busNo != null and query.busNo != ''">
                AND bus_no LIKE CONCAT('%', #{query.busNo}, '%')
            </if>
            <if test="query.idCard != null and query.idCard != ''">
                AND id_card LIKE CONCAT('%', #{query.idCard}, '%')
            </if>
            <if test="query.phone != null and query.phone != ''">
                AND phone LIKE CONCAT('%', #{query.phone}, '%')
            </if>
            <if test="query.genderEnum != null and query.genderEnum != ''">
                AND gender_enum = #{query.genderEnum}
            </if>
            <if test="query.statusEnum != null and query.statusEnum != ''">
                AND status_enum = #{query.statusEnum}
            </if>
            <if test="query.activeFlag != null">
                AND active_flag = #{query.activeFlag}
            </if>
            <if test="query.tempFlag != null">
                AND temp_flag = #{query.tempFlag}
            </if>
            <if test="query.organizationId != null and query.organizationId != ''">
                AND organization_id = #{query.organizationId}
            </if>
            ORDER BY created_at DESC, id DESC
            </script>
            """)
    Page<PatientDO> selectPatientPage(
            Page<PatientDO> page,
            @Param("query") PatientQuery query,
            @Param("tenantId") String tenantId
    );

    /**
     * 按患者ID和当前租户查询可修改的患者档案。
     *
     * @param patientId 患者ID
     * @param tenantId  当前租户ID
     * @return 当前租户下未删除的患者档案
     */
    @Select("""
            SELECT
                id,
                tenant_id,
                created_at,
                updated_at,
                created_by,
                updated_by,
                is_deleted,
                deleted_at,
                deleted_by,
                version,
                bus_no,
                name,
                gender_enum,
                birth_date,
                status_enum,
                active_flag,
                temp_flag,
                id_card,
                marital_status_enum,
                prfs_enum,
                phone,
                address,
                address_province,
                address_city,
                address_district,
                address_street,
                nationality_code,
                py_str,
                wb_str,
                blood_type_abo,
                blood_type_rh,
                work_company,
                native_place,
                country_code,
                link_name,
                link_relation_code,
                link_telcom,
                deceased_date,
                organization_id
            FROM adm_patient
            WHERE id = #{patientId}
              AND tenant_id = #{tenantId}
              AND is_deleted = 0
            LIMIT 1
            """)
    PatientDO selectPatientByIdAndTenant(
            @Param("patientId") String patientId,
            @Param("tenantId") String tenantId
    );

    /**
     * 查询包括已软删数据在内的身份证占用记录，并加排他锁。
     *
     * @param idCard     身份证号
     * @param excludedId 修改时排除的患者ID
     * @param tenantId   当前租户ID
     * @return 占用该身份证号的患者ID
     */
    @Select("""
            SELECT id
            FROM adm_patient
            WHERE id_card = #{idCard}
              AND tenant_id = #{tenantId}
              AND (#{excludedId} IS NULL OR id != #{excludedId})
            LIMIT 1
            FOR UPDATE
            """)
    String selectIdByIdCardForUpdate(
            @Param("idCard") String idCard,
            @Param("excludedId") String excludedId,
            @Param("tenantId") String tenantId
    );

    /**
     * 校验建档机构有效且属于当前租户。
     *
     * @param organizationId 建档机构ID
     * @param tenantId       当前租户ID
     * @return 当前租户ID；机构无效或不属于当前租户时返回空
     */
    @Select("""
            SELECT COALESCE(tenant_id, '')
            FROM adm_organization
            WHERE id = #{organizationId}
              AND is_deleted = 0
              AND active_flag = 1
              AND status_enum = 'ACTIVE'
              AND tenant_id = #{tenantId}
            LIMIT 1
            """)
    String selectTenantIdByActiveOrganizationId(
            @Param("organizationId") String organizationId,
            @Param("tenantId") String tenantId
    );

    /**
     * 锁定并读取患者编号流水。
     */
    @Select("""
            SELECT current_value
            FROM whale_sequence
            WHERE seq_key = #{seqKey}
            FOR UPDATE
            """)
    Long selectSequenceValueForUpdate(@Param("seqKey") String seqKey);

    /**
     * 初始化患者编号流水。
     */
    @Insert("""
            INSERT INTO whale_sequence(seq_key, current_value, updated_at)
            VALUES(#{seqKey}, #{currentValue}, #{updatedAt})
            """)
    int insertSequence(
            @Param("seqKey") String seqKey,
            @Param("currentValue") long currentValue,
            @Param("updatedAt") LocalDateTime updatedAt
    );

    /**
     * 递增患者编号流水。
     */
    @Update("""
            UPDATE whale_sequence
            SET current_value = #{currentValue},
                updated_at = #{updatedAt}
            WHERE seq_key = #{seqKey}
            """)
    int updateSequence(
            @Param("seqKey") String seqKey,
            @Param("currentValue") long currentValue,
            @Param("updatedAt") LocalDateTime updatedAt
    );

    /**
     * 查询患者是否存在进行中的就诊记录（挂号/住院）。
     * <p>排除已结束、已取消、录入错误等终态就诊。</p>
     *
     * @param patientId 患者ID
     * @return 进行中的就诊记录数
     */
    @Select("""
            SELECT COUNT(1)
            FROM adm_encounter
            WHERE patient_id = #{patientId}
              AND is_deleted = 0
              AND status_enum NOT IN ('FINISHED', 'CANCELLED', 'ENTERED_IN_ERROR')
              AND tenant_id = #{tenantId}
            """)
    int countActiveEncounters(@Param("patientId") String patientId,@Param("tenantId") String tenantId);


}
