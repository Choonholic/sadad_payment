// TMS.h

#pragma once

#include "Definitions.h"

// Commands

#define TMS_MSG_COM_BACKUP			301
#define TMS_MSG_COM_RESTORE			302
#define TMS_MSG_COM_ONLINE			303
#define TMS_MSG_COM_OFFLINE			304

// Messages

#define TMS_MSG_REQ_PARAMETERS		305
#define TMS_MSG_REQ_CONFIG_REFRESH	306
#define TMS_MSG_REQ_RESOURCE		401
#define TMS_MSG_REQ_RELEASE			402
#define TMS_MSG_REQ_EXIT			403

#define TMS_MSG_RES_PARAMETERS		315
#define TMS_MSG_RES_CONFIG_REFRESH	316
#define TMS_MSG_RES_RESOURCE		411
#define TMS_MSG_RES_RELEASE			412
#define TMS_MSG_RES_EXIT			413

// Response Codes

#define TMS_RES_SUCCESS				201
#define TMS_RES_ERROR				202

// Package Name

#define TMS_PACKAGE_NAME			L"SADADPay"

BOOL	LoadTMSConfigXML(TMS_UPDATE_INFO* pUpdateInfo);
BOOL	LoadTMSUpdateXML(TMS_UPDATE_INFO* pUpdateInfo);
BOOL	SaveTMSUpdateXML(TMS_UPDATE_INFO* pUpdateInfo);
