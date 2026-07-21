---
title: 前后端API接口文档 v1.0.0
language_tabs:
  - shell: Shell
  - http: HTTP
  - javascript: JavaScript
  - ruby: Ruby
  - python: Python
  - php: PHP
  - java: Java
  - go: Go
toc_footers: []
includes: []
search: true
highlight_theme: darkula
headingLevel: 2

---

<!-- Generator: Widdershins v4.0.1 -->

<h1 id="-api-">前后端API接口文档 v1.0.0</h1>

> Scroll down for code samples, example requests and responses. Select a language for code samples from the tabs above or the mobile navigation menu.

用于前后端交互，提供一套API说明

Base URLs:

* <a href="http://localhost:10003/template">http://localhost:10003/template</a>

<a href="https://space.bilibili.com/1653229811/?spm_id_from=333.999.0.0">Terms of service</a>
Email: <a href="mailto:01xq@mail.com">零壹星球</a> Web: <a href="https://space.bilibili.com/1653229811/?spm_id_from=333.999.0.0">零壹星球</a> 
License: <a href="https://www.apache.org/licenses/LICENSE-2.0.html">Apache 2.0</a>

# Authentication

- HTTP Authentication, scheme: bearer 

<h1 id="-api--order-template">order-template</h1>

组套模板接口

## detail

<a id="opIddetail"></a>

> Code samples

```shell
# You can also use wget
curl -X GET http://localhost:10003/template/{id}/detail \
  -H 'Accept: */*' \
  -H 'Authorization: Bearer {access-token}'

```

```http
GET http://localhost:10003/template/{id}/detail HTTP/1.1
Host: localhost:10003
Accept: */*

```

```javascript

const headers = {
  'Accept':'*/*',
  'Authorization':'Bearer {access-token}'
};

fetch('http://localhost:10003/template/{id}/detail',
{
  method: 'GET',

  headers: headers
})
.then(function(res) {
    return res.json();
}).then(function(body) {
    console.log(body);
});

```

```ruby
require 'rest-client'
require 'json'

headers = {
  'Accept' => '*/*',
  'Authorization' => 'Bearer {access-token}'
}

result = RestClient.get 'http://localhost:10003/template/{id}/detail',
  params: {
  }, headers: headers

p JSON.parse(result)

```

```python
import requests
headers = {
  'Accept': '*/*',
  'Authorization': 'Bearer {access-token}'
}

r = requests.get('http://localhost:10003/template/{id}/detail', headers = headers)

print(r.json())

```

```php
<?php

require 'vendor/autoload.php';

$headers = array(
    'Accept' => '*/*',
    'Authorization' => 'Bearer {access-token}',
);

$client = new \GuzzleHttp\Client();

// Define array of request body.
$request_body = array();

try {
    $response = $client->request('GET','http://localhost:10003/template/{id}/detail', array(
        'headers' => $headers,
        'json' => $request_body,
       )
    );
    print_r($response->getBody()->getContents());
 }
 catch (\GuzzleHttp\Exception\BadResponseException $e) {
    // handle exception or api errors.
    print_r($e->getMessage());
 }

 // ...

```

```java
URL obj = new URL("http://localhost:10003/template/{id}/detail");
HttpURLConnection con = (HttpURLConnection) obj.openConnection();
con.setRequestMethod("GET");
int responseCode = con.getResponseCode();
BufferedReader in = new BufferedReader(
    new InputStreamReader(con.getInputStream()));
String inputLine;
StringBuffer response = new StringBuffer();
while ((inputLine = in.readLine()) != null) {
    response.append(inputLine);
}
in.close();
System.out.println(response.toString());

```

```go
package main

import (
       "bytes"
       "net/http"
)

func main() {

    headers := map[string][]string{
        "Accept": []string{"*/*"},
        "Authorization": []string{"Bearer {access-token}"},
    }

    data := bytes.NewBuffer([]byte{jsonReq})
    req, err := http.NewRequest("GET", "http://localhost:10003/template/{id}/detail", data)
    req.Header = headers

    client := &http.Client{}
    resp, err := client.Do(req)
    // ...
}

```

`GET /{id}/detail`

*获取组套详情（含明细行）*

<h3 id="detail-parameters">Parameters</h3>

|Name|In|Type|Required|Description|
|---|---|---|---|---|
|id|path|string|true|组套模板 ID|

> Example responses

> 200 Response

<h3 id="detail-responses">Responses</h3>

|Status|Meaning|Description|Schema|
|---|---|---|---|
|200|[OK](https://tools.ietf.org/html/rfc7231#section-6.3.1)|OK|[JsonVOOrderTemplateDetailVO](#schemajsonvoordertemplatedetailvo)|

<aside class="warning">
To perform this operation, you must be authenticated by means of one of the following methods:
Authorization
</aside>

## listNames

<a id="opIdlistNames"></a>

> Code samples

```shell
# You can also use wget
curl -X GET http://localhost:10003/template/names \
  -H 'Accept: */*' \
  -H 'Authorization: Bearer {access-token}'

```

```http
GET http://localhost:10003/template/names HTTP/1.1
Host: localhost:10003
Accept: */*

```

```javascript

const headers = {
  'Accept':'*/*',
  'Authorization':'Bearer {access-token}'
};

fetch('http://localhost:10003/template/names',
{
  method: 'GET',

  headers: headers
})
.then(function(res) {
    return res.json();
}).then(function(body) {
    console.log(body);
});

```

```ruby
require 'rest-client'
require 'json'

headers = {
  'Accept' => '*/*',
  'Authorization' => 'Bearer {access-token}'
}

result = RestClient.get 'http://localhost:10003/template/names',
  params: {
  }, headers: headers

p JSON.parse(result)

```

```python
import requests
headers = {
  'Accept': '*/*',
  'Authorization': 'Bearer {access-token}'
}

r = requests.get('http://localhost:10003/template/names', headers = headers)

print(r.json())

```

```php
<?php

require 'vendor/autoload.php';

$headers = array(
    'Accept' => '*/*',
    'Authorization' => 'Bearer {access-token}',
);

$client = new \GuzzleHttp\Client();

// Define array of request body.
$request_body = array();

try {
    $response = $client->request('GET','http://localhost:10003/template/names', array(
        'headers' => $headers,
        'json' => $request_body,
       )
    );
    print_r($response->getBody()->getContents());
 }
 catch (\GuzzleHttp\Exception\BadResponseException $e) {
    // handle exception or api errors.
    print_r($e->getMessage());
 }

 // ...

```

```java
URL obj = new URL("http://localhost:10003/template/names");
HttpURLConnection con = (HttpURLConnection) obj.openConnection();
con.setRequestMethod("GET");
int responseCode = con.getResponseCode();
BufferedReader in = new BufferedReader(
    new InputStreamReader(con.getInputStream()));
String inputLine;
StringBuffer response = new StringBuffer();
while ((inputLine = in.readLine()) != null) {
    response.append(inputLine);
}
in.close();
System.out.println(response.toString());

```

```go
package main

import (
       "bytes"
       "net/http"
)

func main() {

    headers := map[string][]string{
        "Accept": []string{"*/*"},
        "Authorization": []string{"Bearer {access-token}"},
    }

    data := bytes.NewBuffer([]byte{jsonReq})
    req, err := http.NewRequest("GET", "http://localhost:10003/template/names", data)
    req.Header = headers

    client := &http.Client{}
    resp, err := client.Do(req)
    // ...
}

```

`GET /names`

*获取组套名称列表*

<h3 id="listnames-parameters">Parameters</h3>

|Name|In|Type|Required|Description|
|---|---|---|---|---|
|sceneEnum|query|string|false|适用场景（OUTPATIENT/INPATIENT/GENERAL）|
|templateKindEnum|query|string|false|组套种类（GENERAL/TCM/PRICING）|
|practitionerId|query|string|false|当前医生ID|
|deptId|query|string|false|当前科室 ID|

> Example responses

> 200 Response

<h3 id="listnames-responses">Responses</h3>

|Status|Meaning|Description|Schema|
|---|---|---|---|
|200|[OK](https://tools.ietf.org/html/rfc7231#section-6.3.1)|OK|[JsonVOListOrderTemplateNameVO](#schemajsonvolistordertemplatenamevo)|

<aside class="warning">
To perform this operation, you must be authenticated by means of one of the following methods:
Authorization
</aside>

## list

<a id="opIdlist"></a>

> Code samples

```shell
# You can also use wget
curl -X GET http://localhost:10003/template/list \
  -H 'Accept: */*' \
  -H 'Authorization: Bearer {access-token}'

```

```http
GET http://localhost:10003/template/list HTTP/1.1
Host: localhost:10003
Accept: */*

```

```javascript

const headers = {
  'Accept':'*/*',
  'Authorization':'Bearer {access-token}'
};

fetch('http://localhost:10003/template/list',
{
  method: 'GET',

  headers: headers
})
.then(function(res) {
    return res.json();
}).then(function(body) {
    console.log(body);
});

```

```ruby
require 'rest-client'
require 'json'

headers = {
  'Accept' => '*/*',
  'Authorization' => 'Bearer {access-token}'
}

result = RestClient.get 'http://localhost:10003/template/list',
  params: {
  }, headers: headers

p JSON.parse(result)

```

```python
import requests
headers = {
  'Accept': '*/*',
  'Authorization': 'Bearer {access-token}'
}

r = requests.get('http://localhost:10003/template/list', headers = headers)

print(r.json())

```

```php
<?php

require 'vendor/autoload.php';

$headers = array(
    'Accept' => '*/*',
    'Authorization' => 'Bearer {access-token}',
);

$client = new \GuzzleHttp\Client();

// Define array of request body.
$request_body = array();

try {
    $response = $client->request('GET','http://localhost:10003/template/list', array(
        'headers' => $headers,
        'json' => $request_body,
       )
    );
    print_r($response->getBody()->getContents());
 }
 catch (\GuzzleHttp\Exception\BadResponseException $e) {
    // handle exception or api errors.
    print_r($e->getMessage());
 }

 // ...

```

```java
URL obj = new URL("http://localhost:10003/template/list");
HttpURLConnection con = (HttpURLConnection) obj.openConnection();
con.setRequestMethod("GET");
int responseCode = con.getResponseCode();
BufferedReader in = new BufferedReader(
    new InputStreamReader(con.getInputStream()));
String inputLine;
StringBuffer response = new StringBuffer();
while ((inputLine = in.readLine()) != null) {
    response.append(inputLine);
}
in.close();
System.out.println(response.toString());

```

```go
package main

import (
       "bytes"
       "net/http"
)

func main() {

    headers := map[string][]string{
        "Accept": []string{"*/*"},
        "Authorization": []string{"Bearer {access-token}"},
    }

    data := bytes.NewBuffer([]byte{jsonReq})
    req, err := http.NewRequest("GET", "http://localhost:10003/template/list", data)
    req.Header = headers

    client := &http.Client{}
    resp, err := client.Do(req)
    // ...
}

```

`GET /list`

*获取组套列表（分页）*

<h3 id="list-parameters">Parameters</h3>

|Name|In|Type|Required|Description|
|---|---|---|---|---|
|templateKindEnum|query|string|false|组套种类（tab 切换：GENERAL/TCM/PRICING）|
|keyword|query|string|false|关键词（按名称模糊搜索）|
|bindingEnum|query|string|false|绑定类型筛选（PERSONAL/DEPT/GLOBAL）|
|sceneEnum|query|string|false|适用场景筛选（OUTPATIENT/INPATIENT/GENERAL）|
|statusEnum|query|string|false|状态筛选（DRAFT/ACTIVE/INACTIVE）|
|pageIndex|query|string|false|查询页码|
|pageSize|query|string|false|查询条数|

> Example responses

> 200 Response

<h3 id="list-responses">Responses</h3>

|Status|Meaning|Description|Schema|
|---|---|---|---|
|200|[OK](https://tools.ietf.org/html/rfc7231#section-6.3.1)|OK|[JsonVOPageDTOOrderTemplateListVO](#schemajsonvopagedtoordertemplatelistvo)|

<aside class="warning">
To perform this operation, you must be authenticated by means of one of the following methods:
Authorization
</aside>

# Schemas

<h2 id="tocS_JsonVOOrderTemplateDetailVO">JsonVOOrderTemplateDetailVO</h2>
<!-- backwards compatibility -->
<a id="schemajsonvoordertemplatedetailvo"></a>
<a id="schema_JsonVOOrderTemplateDetailVO"></a>
<a id="tocSjsonvoordertemplatedetailvo"></a>
<a id="tocsjsonvoordertemplatedetailvo"></a>

```json
{
  "code": 10000,
  "message": "提示消息内容",
  "data": {
    "id": "string",
    "name": "string",
    "templateCode": "string",
    "bindingEnum": "string",
    "sceneEnum": "string",
    "templateKindEnum": "string",
    "statusEnum": "string",
    "templateSourceEnum": "string",
    "description": "string",
    "remark": "string",
    "sortWeight": 0,
    "defaultChineseHerbsDoseQuantity": 0,
    "defaultSufferingFlag": true,
    "createdAt": "2019-08-24T14:15:22Z",
    "updatedAt": "2019-08-24T14:15:22Z",
    "items": [
      {
        "id": "string",
        "lineKindEnum": "string",
        "medicationDefinitionId": "string",
        "activityDefinitionId": "string",
        "deviceDefinitionId": "string",
        "groupKey": "string",
        "sortOrder": 0,
        "defaultQuantity": 0,
        "defaultUnitCode": "string",
        "defaultDays": 0,
        "frequencyId": "string",
        "routeCode": "string",
        "dosageText": "string",
        "defaultPerformOrgId": "string",
        "lineRemark": "string",
        "chineseHerbsDoseQuantity": 0,
        "sufferingFlag": true
      }
    ]
  },
  "status": "UNAUTHORIZED"
}

```

### Properties

|Name|Type|Required|Restrictions|Description|
|---|---|---|---|---|
|code|integer(int32)|true|none|状态码|
|message|string|true|none|提示消息|
|data|[OrderTemplateDetailVO](#schemaordertemplatedetailvo)|false|none|数据对象|
|status|string|false|write-only|none|

#### Enumerated Values

|Property|Value|
|---|---|
|status|UNAUTHORIZED|
|status|FORBIDDEN|
|status|SERVER_ERROR|
|status|PARAMS_INVALID|
|status|CONTENT_TYPE_ERR|
|status|API_UN_IMPL|
|status|SERVER_BUSY|
|status|FAIL|
|status|SUCCESS|

<h2 id="tocS_OrderTemplateDetailVO">OrderTemplateDetailVO</h2>
<!-- backwards compatibility -->
<a id="schemaordertemplatedetailvo"></a>
<a id="schema_OrderTemplateDetailVO"></a>
<a id="tocSordertemplatedetailvo"></a>
<a id="tocsordertemplatedetailvo"></a>

```json
{
  "id": "string",
  "name": "string",
  "templateCode": "string",
  "bindingEnum": "string",
  "sceneEnum": "string",
  "templateKindEnum": "string",
  "statusEnum": "string",
  "templateSourceEnum": "string",
  "description": "string",
  "remark": "string",
  "sortWeight": 0,
  "defaultChineseHerbsDoseQuantity": 0,
  "defaultSufferingFlag": true,
  "createdAt": "2019-08-24T14:15:22Z",
  "updatedAt": "2019-08-24T14:15:22Z",
  "items": [
    {
      "id": "string",
      "lineKindEnum": "string",
      "medicationDefinitionId": "string",
      "activityDefinitionId": "string",
      "deviceDefinitionId": "string",
      "groupKey": "string",
      "sortOrder": 0,
      "defaultQuantity": 0,
      "defaultUnitCode": "string",
      "defaultDays": 0,
      "frequencyId": "string",
      "routeCode": "string",
      "dosageText": "string",
      "defaultPerformOrgId": "string",
      "lineRemark": "string",
      "chineseHerbsDoseQuantity": 0,
      "sufferingFlag": true
    }
  ]
}

```

数据对象

### Properties

|Name|Type|Required|Restrictions|Description|
|---|---|---|---|---|
|id|string|false|none|组套模板 ID|
|name|string|false|none|模板名称|
|templateCode|string|false|none|模板编码|
|bindingEnum|string|false|none|绑定类型|
|sceneEnum|string|false|none|适用场景|
|templateKindEnum|string|false|none|组套种类|
|statusEnum|string|false|none|状态|
|templateSourceEnum|string|false|none|模板来源|
|description|string|false|none|描述|
|remark|string|false|none|备注|
|sortWeight|integer(int32)|false|none|排序权重|
|defaultChineseHerbsDoseQuantity|number|false|none|TCM 默认付数|
|defaultSufferingFlag|boolean|false|none|TCM 默认代煎标志|
|createdAt|string(date-time)|false|none|创建时间|
|updatedAt|string(date-time)|false|none|更新时间|
|items|[[OrderTemplateLineVO](#schemaordertemplatelinevo)]|false|none|组套明细行列表|

<h2 id="tocS_OrderTemplateLineVO">OrderTemplateLineVO</h2>
<!-- backwards compatibility -->
<a id="schemaordertemplatelinevo"></a>
<a id="schema_OrderTemplateLineVO"></a>
<a id="tocSordertemplatelinevo"></a>
<a id="tocsordertemplatelinevo"></a>

```json
{
  "id": "string",
  "lineKindEnum": "string",
  "medicationDefinitionId": "string",
  "activityDefinitionId": "string",
  "deviceDefinitionId": "string",
  "groupKey": "string",
  "sortOrder": 0,
  "defaultQuantity": 0,
  "defaultUnitCode": "string",
  "defaultDays": 0,
  "frequencyId": "string",
  "routeCode": "string",
  "dosageText": "string",
  "defaultPerformOrgId": "string",
  "lineRemark": "string",
  "chineseHerbsDoseQuantity": 0,
  "sufferingFlag": true
}

```

组套明细行列表

### Properties

|Name|Type|Required|Restrictions|Description|
|---|---|---|---|---|
|id|string|false|none|明细行 ID|
|lineKindEnum|string|false|none|行类型枚举|
|medicationDefinitionId|string|false|none|药品定义 ID|
|activityDefinitionId|string|false|none|诊疗活动定义 ID|
|deviceDefinitionId|string|false|none|耗材设备定义 ID|
|groupKey|string|false|none|分组 key|
|sortOrder|integer(int32)|false|none|排序号|
|defaultQuantity|number|false|none|默认数量|
|defaultUnitCode|string|false|none|默认单位编码|
|defaultDays|integer(int32)|false|none|默认天数|
|frequencyId|string|false|none|频次 ID|
|routeCode|string|false|none|给药途径编码|
|dosageText|string|false|none|剂量说明|
|defaultPerformOrgId|string|false|none|默认执行科室 ID|
|lineRemark|string|false|none|行备注|
|chineseHerbsDoseQuantity|number|false|none|TCM 付数|
|sufferingFlag|boolean|false|none|TCM 代煎标志|

<h2 id="tocS_JsonVOListOrderTemplateNameVO">JsonVOListOrderTemplateNameVO</h2>
<!-- backwards compatibility -->
<a id="schemajsonvolistordertemplatenamevo"></a>
<a id="schema_JsonVOListOrderTemplateNameVO"></a>
<a id="tocSjsonvolistordertemplatenamevo"></a>
<a id="tocsjsonvolistordertemplatenamevo"></a>

```json
{
  "code": 10000,
  "message": "提示消息内容",
  "data": [
    {
      "id": "string",
      "name": "string",
      "bindingEnum": "string",
      "templateKindEnum": "string",
      "sceneEnum": "string"
    }
  ],
  "status": "UNAUTHORIZED"
}

```

### Properties

|Name|Type|Required|Restrictions|Description|
|---|---|---|---|---|
|code|integer(int32)|true|none|状态码|
|message|string|true|none|提示消息|
|data|[[OrderTemplateNameVO](#schemaordertemplatenamevo)]|false|none|数据对象|
|status|string|false|write-only|none|

#### Enumerated Values

|Property|Value|
|---|---|
|status|UNAUTHORIZED|
|status|FORBIDDEN|
|status|SERVER_ERROR|
|status|PARAMS_INVALID|
|status|CONTENT_TYPE_ERR|
|status|API_UN_IMPL|
|status|SERVER_BUSY|
|status|FAIL|
|status|SUCCESS|

<h2 id="tocS_OrderTemplateNameVO">OrderTemplateNameVO</h2>
<!-- backwards compatibility -->
<a id="schemaordertemplatenamevo"></a>
<a id="schema_OrderTemplateNameVO"></a>
<a id="tocSordertemplatenamevo"></a>
<a id="tocsordertemplatenamevo"></a>

```json
{
  "id": "string",
  "name": "string",
  "bindingEnum": "string",
  "templateKindEnum": "string",
  "sceneEnum": "string"
}

```

数据对象

### Properties

|Name|Type|Required|Restrictions|Description|
|---|---|---|---|---|
|id|string|false|none|组套模板 ID|
|name|string|false|none|组套名称|
|bindingEnum|string|false|none|绑定类型（前端按此字段分三栏）|
|templateKindEnum|string|false|none|组套种类|
|sceneEnum|string|false|none|适用场景|

<h2 id="tocS_JsonVOPageDTOOrderTemplateListVO">JsonVOPageDTOOrderTemplateListVO</h2>
<!-- backwards compatibility -->
<a id="schemajsonvopagedtoordertemplatelistvo"></a>
<a id="schema_JsonVOPageDTOOrderTemplateListVO"></a>
<a id="tocSjsonvopagedtoordertemplatelistvo"></a>
<a id="tocsjsonvopagedtoordertemplatelistvo"></a>

```json
{
  "code": 10000,
  "message": "提示消息内容",
  "data": {
    "pageIndex": 1,
    "pageSize": 10,
    "total": 100000,
    "pages": 100,
    "rows": [
      {
        "id": "string",
        "name": "string",
        "templateCode": "string",
        "bindingEnum": "string",
        "sceneEnum": "string",
        "templateKindEnum": "string",
        "statusEnum": "string",
        "sortWeight": 0,
        "createdAt": "2019-08-24T14:15:22Z",
        "updatedAt": "2019-08-24T14:15:22Z"
      }
    ]
  },
  "status": "UNAUTHORIZED"
}

```

### Properties

|Name|Type|Required|Restrictions|Description|
|---|---|---|---|---|
|code|integer(int32)|true|none|状态码|
|message|string|true|none|提示消息|
|data|[PageDTOOrderTemplateListVO](#schemapagedtoordertemplatelistvo)|false|none|数据对象|
|status|string|false|write-only|none|

#### Enumerated Values

|Property|Value|
|---|---|
|status|UNAUTHORIZED|
|status|FORBIDDEN|
|status|SERVER_ERROR|
|status|PARAMS_INVALID|
|status|CONTENT_TYPE_ERR|
|status|API_UN_IMPL|
|status|SERVER_BUSY|
|status|FAIL|
|status|SUCCESS|

<h2 id="tocS_OrderTemplateListVO">OrderTemplateListVO</h2>
<!-- backwards compatibility -->
<a id="schemaordertemplatelistvo"></a>
<a id="schema_OrderTemplateListVO"></a>
<a id="tocSordertemplatelistvo"></a>
<a id="tocsordertemplatelistvo"></a>

```json
{
  "id": "string",
  "name": "string",
  "templateCode": "string",
  "bindingEnum": "string",
  "sceneEnum": "string",
  "templateKindEnum": "string",
  "statusEnum": "string",
  "sortWeight": 0,
  "createdAt": "2019-08-24T14:15:22Z",
  "updatedAt": "2019-08-24T14:15:22Z"
}

```

当前页数据列表

### Properties

|Name|Type|Required|Restrictions|Description|
|---|---|---|---|---|
|id|string|false|none|组套模板 ID|
|name|string|false|none|模板名称|
|templateCode|string|false|none|模板编码|
|bindingEnum|string|false|none|绑定类型|
|sceneEnum|string|false|none|适用场景|
|templateKindEnum|string|false|none|组套种类|
|statusEnum|string|false|none|状态|
|sortWeight|integer(int32)|false|none|排序权重|
|createdAt|string(date-time)|false|none|创建时间|
|updatedAt|string(date-time)|false|none|更新时间|

<h2 id="tocS_PageDTOOrderTemplateListVO">PageDTOOrderTemplateListVO</h2>
<!-- backwards compatibility -->
<a id="schemapagedtoordertemplatelistvo"></a>
<a id="schema_PageDTOOrderTemplateListVO"></a>
<a id="tocSpagedtoordertemplatelistvo"></a>
<a id="tocspagedtoordertemplatelistvo"></a>

```json
{
  "pageIndex": 1,
  "pageSize": 10,
  "total": 100000,
  "pages": 100,
  "rows": [
    {
      "id": "string",
      "name": "string",
      "templateCode": "string",
      "bindingEnum": "string",
      "sceneEnum": "string",
      "templateKindEnum": "string",
      "statusEnum": "string",
      "sortWeight": 0,
      "createdAt": "2019-08-24T14:15:22Z",
      "updatedAt": "2019-08-24T14:15:22Z"
    }
  ]
}

```

数据对象

### Properties

|Name|Type|Required|Restrictions|Description|
|---|---|---|---|---|
|pageIndex|integer(int64)|true|none|当前页码|
|pageSize|integer(int64)|true|none|每页显示最大数据条数|
|total|integer(int64)|true|none|总条数|
|pages|integer(int64)|true|none|总页数|
|rows|[[OrderTemplateListVO](#schemaordertemplatelistvo)]|false|none|当前页数据列表|

