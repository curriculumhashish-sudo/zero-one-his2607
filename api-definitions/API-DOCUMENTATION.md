# 接口文档

三个接口，返回格式都是 `{ code, message, data }`。

---

## 1. 入库单删除

地址：`POST /api/app/inventory/{module}/stock-in/{busNo}/delete`

module 填 warehouse（药库）或 pharmacy（药房），busNo 是单据号，reason 选填。

```bash
curl -X POST "http://localhost/api/app/inventory/warehouse/stock-in/SI202607190001/delete?reason=录入错误"
```

成功返回：
```json
{
  "code": 200,
  "data": { "busNo": "SI202607190001", "deletedLines": 3, "unlockedLocks": 3 }
}
```

---

## 2. 盘点单删除

地址：`POST /api/app/inventory/{module}/stocktake/{busNo}/delete`

module 比入库单多一个 department（科室）。

```bash
curl -X POST "http://localhost/api/app/inventory/pharmacy/stocktake/ST202607190001/delete?reason=数据错了"
```

返回格式一样。

---

## 3. 门诊批量发药

地址：`POST /api/app/outpatientPharmacyDispense/batchDispense`

请求体：
```json
{
  "dispenseIds": ["322663105157926912"],
  "preparerId": "320000000000000001",
  "locationId": "320000000000000100",
  "lotSelections": [
    { "dispenseId": "322663105157926912", "lotNumber": "20260715-001" }
  ],
  "traceCodes": [
    { "dispenseId": "322663105157926912", "traceNo": "81234567890123456789" }
  ]
}
```

成功返回：
```json
{
  "code": 200,
  "data": { "totalCount": 2, "successCount": 2, "failCount": 0, "errors": [] }
}
```
