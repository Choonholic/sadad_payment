// ViewCharge.cpp

#include "stdafx.h"
#include "SADADPay.h"
#include "ChildView.h"
#include "PNGRes.h"
#include "Utilities.h"
#include "StringRes.h"

void CChildView::InitializeMenuProvider()
{
	InitializeMenuBase(&m_menuInfoProvider, MENU_PROVIDER, IDS_SELECT_PROVIDER, m_Parameters.nMaxProviders);

	for (int i = 0; i < m_menuInfoProvider.nMaxItems; i++)
	{
		SetMenuItem(&m_menuInfoProvider, i, m_Parameters.providers[i].providerName);
	}
}

void CChildView::InitializeMenuCategory()
{
	CString	strE;
	CString	strF;

	InitializeMenuBase(&m_menuInfoCategory, MENU_CATEGORY, IDS_SELECT_CATEGORY, m_Parameters.providers[m_menuInfoProvider.nCurItem].nVoucherCount);

	for (int i = 0; i < m_menuInfoCategory.nMaxItems; i++)
	{
		strE.Format(L"%lld %s", m_Parameters.providers[m_menuInfoProvider.nCurItem].vouchers[i].llAmount, LoadEnglishText(IDS_RIALS));
		strF.Format(L"%lld %s", m_Parameters.providers[m_menuInfoProvider.nCurItem].vouchers[i].llAmount, LoadFarsiText(IDS_RIALS));

		SetMenuItem(&m_menuInfoCategory, i, strE, strF);
	}
}

void CChildView::ExecuteMenuProvider()
{
	wcscpy(m_TransInfo.szProviderID, m_Parameters.providers[m_menuInfoProvider.nCurItem].szProviderID);

	InitializeMenuCategory();
	SetViewMode(VIEW_MENU_CATEGORY);
}

void CChildView::ExecuteMenuCategory()
{
	swprintf(m_TransInfo.szCategoryID, L"%d", m_Parameters.providers[m_menuInfoProvider.nCurItem].vouchers[m_menuInfoCategory.nCurItem].nCID);
	swprintf(m_TransInfo.szAmount, L"%lld", m_Parameters.providers[m_menuInfoProvider.nCurItem].vouchers[m_menuInfoCategory.nCurItem].llAmount);

	switch (GetTransactionMode())
	{
	case TRANSACTION_CHARGE:
		{
			SetViewMode(VIEW_CHARGE_CONFIRM);
			ForcePaintFlag();
		}
		break;
	case TRANSACTION_SELL_CHARGE:
		{
			CHARGE_INFO	chargeInfo;

			memset(&chargeInfo , 0, sizeof(CHARGE_INFO));

			if (m_database.SelectChargeRecord(m_TransInfo.szProviderID, m_TransInfo.szCategoryID, &chargeInfo))
			{
				SetPrintInfo(&m_PrintInfo, RECEIPT_SELL_CHARGE, &chargeInfo, &m_Parameters, RSEQ_CUSTOMER_ONLY);
				SetViewMode(VIEW_PRINT);
				ForcePaintFlag();
			}
			else
			{
				SetTransactionMode(TRANSACTION_SELL_CHARGE);
				ShowMessage(ERR_NO_RECORD_FOUND, MESSAGE_INFO);
			}
		}
		break;
	}
}
