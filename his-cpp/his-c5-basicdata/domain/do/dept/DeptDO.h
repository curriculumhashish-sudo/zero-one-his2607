#pragma once
<<<<<<< HEAD
=======

>>>>>>> 9c70232bf8f9e68687532bbbc58b5f56c13b0ce0
#ifndef _DEPT_DO_
#define _DEPT_DO_
#include "../DoInclude.h"

<<<<<<< HEAD
/**
 * ¿ÆÊÒ»ú¹¹±í adm_organization Êı¾İ¿âÊµÌåDO
 * ÍêÕûÓ³Éä±íËùÓĞ×Ö¶Î£¬×ñÑ­ÏîÄ¿MYSQL_SYNTHESIZEºê¹æ·¶
 */
class DeptDO : public BaseDO
{
	// Ö÷¼ü id bigint
	MYSQL_SYNTHESIZE(int64_t, id, Id);
	// ×â»§ID
	MYSQL_SYNTHESIZE(int64_t, tenant_id, TenantId);
	// ´´½¨Ê±¼ä
	MYSQL_SYNTHESIZE(string, created_at, CreatedAt);
	// ¸üĞÂÊ±¼ä
	MYSQL_SYNTHESIZE(string, updated_at, UpdatedAt);
	// ´´½¨ÈËID
	MYSQL_SYNTHESIZE(int64_t, created_by, CreatedBy);
	// ×îºó¸üĞÂÈËID
	MYSQL_SYNTHESIZE(int64_t, updated_by, UpdatedBy);
	// ÈíÉ¾³ı±ê¼Ç bool
	MYSQL_SYNTHESIZE(bool, is_deleted, IsDeleted);
	// É¾³ıÊ±¼ä
	MYSQL_SYNTHESIZE(string, deleted_at, DeletedAt);
	// É¾³ıÈËID
	MYSQL_SYNTHESIZE(int64_t, deleted_by, DeletedBy);
	// ÀÖ¹ÛËø°æ±¾ºÅ int
	MYSQL_SYNTHESIZE(int32_t, version, Version);
	// ¿ÆÊÒÃû³Æ
	MYSQL_SYNTHESIZE(string, name, Name);
	// bus±àºÅ
	MYSQL_SYNTHESIZE(string, bus_no, BusNo);
	// ¸¸¼¶¿ÆÊÒID
	MYSQL_SYNTHESIZE(int64_t, parent_id, ParentId);
	// typeÃ¶¾Ù
	MYSQL_SYNTHESIZE(string, type_enum, TypeEnum);
	// class·ÖÀàÃ¶¾Ù
	MYSQL_SYNTHESIZE(string, class_enum, ClassEnum);
	// ÆôÍ£×´Ì¬Ã¶¾Ù ACTIVE/INACTIVE
	MYSQL_SYNTHESIZE(string, status_enum, StatusEnum);
	// ÆôÍ£²¼¶û±ê¼Ç true/false
	MYSQL_SYNTHESIZE(bool, active_flag, ActiveFlag);
	// ÏÔÊ¾ÅÅĞòĞòºÅ
	MYSQL_SYNTHESIZE(int32_t, display_order, DisplayOrder);
	// Ä¬ÈÏÒ½ÉúID
	MYSQL_SYNTHESIZE(int64_t, def_doctor_id, DefDoctorId);
	// Æ´ÒôÂë
	MYSQL_SYNTHESIZE(string, py_str, PyStr);
	// Îå±ÊÂë
	MYSQL_SYNTHESIZE(string, wb_str, WbStr);
	// Ò½±£±àºÅ
	MYSQL_SYNTHESIZE(string, yb_no, YbNo);
	// Ò½±£Ãû³Æ
	MYSQL_SYNTHESIZE(string, yb_name, YbName);
	// Ò½ÁÆ»ú¹¹ID
	MYSQL_SYNTHESIZE(string, medins_id, MedinsId);
	// Ò½ÁÆ»ú¹¹Ò½±£Çø»®
	MYSQL_SYNTHESIZE(string, medins_admdvs, MedinsAdmdvs);
	// Ò½ÁÆ»ú¹¹ÀàĞÍ
	MYSQL_SYNTHESIZE(string, medins_type, MedinsType);
	// Ò½ÁÆ»ú¹¹µÈ¼¶
	MYSQL_SYNTHESIZE(string, medins_lv, MedinsLv);
	// ¿Æ±ğ
	MYSQL_SYNTHESIZE(string, caty, Caty);

public:
	// ¹¹Ôìº¯Êı£º´«Èë±íÃû adm_organization£¬ÒÀ´Î°ó¶¨ËùÓĞ×Ö¶Î
	DeptDO() : BaseDO("adm_organization")
	{
		// Ö÷¼ü×Ö¶Î id  bigint ÀàĞÍÕ¼Î»·û ll
		MYSQL_ADD_FIELD_PK("id", "ll", id);

		// ÆÕÍ¨×Ö¶ÎÒÀ´Î°ó¶¨£¨ÀàĞÍÕ¼Î»·û¶ÔÕÕSqlSessionºê¹æÔò£©
		MYSQL_ADD_FIELD_NULLABLE("tenant_id", "ll", tenant_id, true);
		MYSQL_ADD_FIELD("created_at", "dt", created_at);
		MYSQL_ADD_FIELD_NULLABLE("updated_at", "dt", updated_at, true);
		MYSQL_ADD_FIELD_NULLABLE("created_by", "ll", created_by, true);
		MYSQL_ADD_FIELD_NULLABLE("updated_by", "ll", updated_by, true);
		MYSQL_ADD_FIELD("is_deleted", "b", is_deleted);
		MYSQL_ADD_FIELD_NULLABLE("deleted_at", "dt", deleted_at, true);
		MYSQL_ADD_FIELD_NULLABLE("deleted_by", "ll", deleted_by, true);
		MYSQL_ADD_FIELD("version", "i", version);
		MYSQL_ADD_FIELD("name", "s", name);
		MYSQL_ADD_FIELD("bus_no", "s", bus_no);
		MYSQL_ADD_FIELD_NULLABLE("parent_id", "ll", parent_id, true);
		MYSQL_ADD_FIELD("type_enum", "s", type_enum);
		MYSQL_ADD_FIELD_NULLABLE("class_enum", "s", class_enum, true);
		MYSQL_ADD_FIELD("status_enum", "s", status_enum);
		MYSQL_ADD_FIELD("active_flag", "b", active_flag);
		MYSQL_ADD_FIELD_NULLABLE("display_order", "i", display_order, true);
		MYSQL_ADD_FIELD_NULLABLE("def_doctor_id", "ll", def_doctor_id, true);
		MYSQL_ADD_FIELD_NULLABLE("py_str", "s", py_str, true);
		MYSQL_ADD_FIELD_NULLABLE("wb_str", "s", wb_str, true);
		MYSQL_ADD_FIELD_NULLABLE("yb_no", "s", yb_no, true);
		MYSQL_ADD_FIELD_NULLABLE("yb_name", "s", yb_name, true);
		MYSQL_ADD_FIELD_NULLABLE("medins_id", "s", medins_id, true);
		MYSQL_ADD_FIELD_NULLABLE("medins_admdvs", "s", medins_admdvs, true);
		MYSQL_ADD_FIELD_NULLABLE("medins_type", "s", medins_type, true);
		MYSQL_ADD_FIELD_NULLABLE("medins_lv", "s", medins_lv, true);
		MYSQL_ADD_FIELD_NULLABLE("caty", "s", caty, true);
	}
};

// ÖÇÄÜÖ¸Õë±ğÃû£¬ÏîÄ¿Í³Ò»¹æ·¶
typedef std::shared_ptr<DeptDO> PtrDeptDO;

=======
class DeptDO : public BaseDO
{
    // ç§‘å®¤ç¼–å·
    MYSQL_SYNTHESIZE(string, id, Id);
    // ç§‘å®¤åç§°
    MYSQL_SYNTHESIZE(string, name, Name);
    // ç±»å‹
    MYSQL_SYNTHESIZE(string, typeEnum, TypeEnum);
    // ç±»åˆ«
    MYSQL_SYNTHESIZE(string, classEnum, ClassEnum);
    // ä¸Šçº§ç»„ç»‡ç¼–å·
    MYSQL_SYNTHESIZE(string, parentId, ParentId);
    // åŒ»ä¿ç¼–ç 
    MYSQL_SYNTHESIZE(string, ybNo, YbNo);
    // åŒ»ä¿åç§°
    MYSQL_SYNTHESIZE(string, ybName, YbName);
    // æœºæ„æ ‡è¯†
    MYSQL_SYNTHESIZE(string, medinsId, MedinsId);
    // åŒ»ä¿åŒºåˆ’
    MYSQL_SYNTHESIZE(string, medinsAdmdvs, MedinsAdmdvs);
    // æœºæ„ç±»å‹
    MYSQL_SYNTHESIZE(string, medinsType, MedinsType);
    // æœºæ„ç­‰çº§
    MYSQL_SYNTHESIZE(string, medinsLv, MedinsLv);
    // æ‹¼éŸ³ç 
    MYSQL_SYNTHESIZE(string, pyStr, PyStr);
    // äº”ç¬”ç 
    MYSQL_SYNTHESIZE(string, wbStr, WbStr);
    // é»˜è®¤åŒ»ç”Ÿç¼–å·
    MYSQL_SYNTHESIZE(string, defDoctorId, DefDoctorId);
    // ç§‘åˆ«
    MYSQL_SYNTHESIZE(string, caty, Caty);
    // æ˜¾ç¤ºé¡ºåº
    MYSQL_SYNTHESIZE(string, displayOrder, DisplayOrder);
    // çŠ¶æ€
    MYSQL_SYNTHESIZE(string, statusEnum, StatusEnum);
    // æ˜¯å¦å¯ç”¨
    MYSQL_SYNTHESIZE(string, activeFlag, ActiveFlag);

    // åˆ›å»ºäºº
    MYSQL_SYNTHESIZE(string, createBy, CreateBy);
    // åˆ›å»ºæ—¶é—´
    MYSQL_SYNTHESIZE(string, createTime, CreateTime);
    // ä¿®æ”¹äºº
    MYSQL_SYNTHESIZE(string, updateBy, UpdateBy);
    // ä¿®æ”¹æ—¶é—´
    MYSQL_SYNTHESIZE(string, updateTime, UpdateTime);
public:
    DeptDO() : BaseDO("table_name")
    {
        // ç§‘å®¤ç¼–å·
        MYSQL_ADD_FIELD_PK("id", "s", id);
        // ç§‘å®¤åç§°
        MYSQL_ADD_FIELD("name", "s", name);
        // ç±»å‹
        MYSQL_ADD_FIELD("type_enum", "s", typeEnum);
        // ç±»åˆ«
        MYSQL_ADD_FIELD("class_enum", "s", classEnum);
        // ä¸Šçº§ç»„ç»‡ç¼–å·
        MYSQL_ADD_FIELD("parent_id", "s", parentId);
        // åŒ»ä¿ç¼–ç 
        MYSQL_ADD_FIELD("yb_no", "s", ybNo);
        // åŒ»ä¿åç§°
        MYSQL_ADD_FIELD("yb_name", "s", ybName);
        // æœºæ„æ ‡è¯†
        MYSQL_ADD_FIELD("medins_id", "s", medinsId);
        // åŒ»ä¿åŒºåˆ’
        MYSQL_ADD_FIELD("medins_admdvs", "s", medinsAdmdvs);
        // æœºæ„ç±»å‹
        MYSQL_ADD_FIELD("medins_type", "s", medinsType);
        // æœºæ„ç­‰çº§
        MYSQL_ADD_FIELD("medins_lv", "s", medinsLv);
        // æ‹¼éŸ³ç 
        MYSQL_ADD_FIELD("py_str", "s", pyStr);
        // äº”ç¬”ç 
        MYSQL_ADD_FIELD("wb_str", "s", wbStr);
        // é»˜è®¤åŒ»ç”Ÿç¼–å·
        MYSQL_ADD_FIELD("def_doctor_id", "s", defDoctorId);
        // ç§‘åˆ«
        MYSQL_ADD_FIELD("caty", "s", caty);
        // æ˜¾ç¤ºé¡ºåº
        MYSQL_ADD_FIELD("display_order", "s", displayOrder);
        // çŠ¶æ€
        MYSQL_ADD_FIELD("status_enum", "s", statusEnum);
        // æ˜¯å¦å¯ç”¨
        MYSQL_ADD_FIELD("active_flag", "s", activeFlag);

        // åˆ›å»ºäºº
        MYSQL_ADD_FIELD("create_by", "s", createBy);
        // åˆ›å»ºæ—¶é—´
        MYSQL_ADD_FIELD("create_time", "s", createTime);
        // ä¿®æ”¹äºº
        MYSQL_ADD_FIELD("update_by", "s", updateBy);
        // ä¿®æ”¹æ—¶é—´
        MYSQL_ADD_FIELD("update_time", "s", updateTime);
    }
};

typedef std::shared_ptr<DeptDO> PtrDeptDO;
>>>>>>> 9c70232bf8f9e68687532bbbc58b5f56c13b0ce0
#endif // !_DEPT_DO_
