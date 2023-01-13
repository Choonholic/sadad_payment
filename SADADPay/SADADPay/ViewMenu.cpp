// ViewMenu.cpp

#include "stdafx.h"
#include "SADADPay.h"
#include "ChildView.h"
#include "PNGRes.h"
#include "StringRes.h"
#include "Utilities.h"

void CChildView::InitializeMenuBase(MENU_INFO* pMenuInfo, MENU_TYPE type, UINT nTitleID, UINT uItemCount, BOOL bHierarchy, VIEW_MODE previousViewMode, TRANSACTION_MODE previousTransactionMode, BOOL bCheckList)
{
	pMenuInfo->menuType					= type;
	pMenuInfo->bCheckType				= bCheckList;
	pMenuInfo->nMaxItems				= uItemCount;

#if MENU_ARROW
	if (MENU_ITEMS_PER_PAGE < pMenuInfo->nMaxItems)
	{
		pMenuInfo->nItemsPerPage		= (MENU_ITEMS_PER_PAGE - 2);
		pMenuInfo->bArrow				= TRUE;
	}
	else
	{
		pMenuInfo->nItemsPerPage		= pMenuInfo->nMaxItems;
		pMenuInfo->bArrow				= FALSE;
	}
#else
	pMenuInfo->nItemsPerPage			= ((MENU_ITEMS_PER_PAGE < pMenuInfo->nMaxItems) ? MENU_ITEMS_PER_PAGE : pMenuInfo->nMaxItems);
	pMenuInfo->bArrow					= FALSE;
#endif

	pMenuInfo->menuArrowStatus			= MENU_ARROW_NONE;
	pMenuInfo->nMaxPages				= (pMenuInfo->nMaxItems + pMenuInfo->nItemsPerPage - 1) / pMenuInfo->nItemsPerPage;
	pMenuInfo->nPrevItem				= 0;
	pMenuInfo->nCurItem					= 0;
	pMenuInfo->nPrevPage				= 0;
	pMenuInfo->nCurPage					= 0;
	pMenuInfo->nTitleID					= nTitleID;
	pMenuInfo->bHierarchy				= bHierarchy;
	pMenuInfo->previousViewMode			= previousViewMode;
	pMenuInfo->previousTransactionMode	= previousTransactionMode;
}

BOOL CChildView::SetMenuItem(MENU_INFO* pMenuInfo, int nIndex, BILINGUAL text, int nStatus) 
{
	if ((nIndex < 0) || (nIndex >= pMenuInfo->nMaxItems))
	{
		return FALSE;
	}

	pMenuInfo->menuItems[nIndex].nMenuSeq	= (nIndex + 1);
	pMenuInfo->menuItems[nIndex].nStatus	= MENU_UNCHECKED;

	wcscpy(pMenuInfo->menuItems[nIndex].MenuTitle.szE, text.szE);
	wcscpy(pMenuInfo->menuItems[nIndex].MenuTitle.szF, text.szF);

	return TRUE;
}

BOOL CChildView::SetMenuItem(MENU_INFO* pMenuInfo, int nIndex, UINT uID, int nStatus) 
{
	if ((nIndex < 0) || (nIndex >= pMenuInfo->nMaxItems))
	{
		return FALSE;
	}

	pMenuInfo->menuItems[nIndex].nMenuSeq	= (nIndex + 1);
	pMenuInfo->menuItems[nIndex].nStatus	= MENU_UNCHECKED;

	wcscpy(pMenuInfo->menuItems[nIndex].MenuTitle.szE, LoadEnglishText(uID));
	wcscpy(pMenuInfo->menuItems[nIndex].MenuTitle.szF, LoadFarsiText(uID));

	return TRUE;
}

BOOL CChildView::SetMenuItem(MENU_INFO* pMenuInfo, int nIndex, LPCWSTR szE, LPCWSTR szF, int nStatus) 
{
	if ((nIndex < 0) || (nIndex >= pMenuInfo->nMaxItems))
	{
		return FALSE;
	}

	pMenuInfo->menuItems[nIndex].nMenuSeq	= (nIndex + 1);
	pMenuInfo->menuItems[nIndex].nStatus	= MENU_UNCHECKED;

	wcscpy(pMenuInfo->menuItems[nIndex].MenuTitle.szE, szE);
	wcscpy(pMenuInfo->menuItems[nIndex].MenuTitle.szF, szF);

	return TRUE;
}

void CChildView::InitializeMenu(MENU_INFO* pMenuInfo, MENU_TYPE type, UINT nTitleID, UINT uFirstID, UINT uLastID, BOOL bHierarchy, VIEW_MODE previousViewMode, TRANSACTION_MODE previousTransactionMode, BOOL bCheckList)
{
	InitializeMenuBase(pMenuInfo, type, nTitleID, (uLastID - uFirstID + 1), bHierarchy, previousViewMode, previousTransactionMode, bCheckList);

	for (int i = 0; i < pMenuInfo->nMaxItems; i++)
	{
		SetMenuItem(pMenuInfo, i, uFirstID + i);
	}
}

void CChildView::ExecuteMenu(MENU_INFO* pMenuInfo)
{
	switch (pMenuInfo->menuType)
	{
	case MENU_MAIN:
		{
			ExecuteMenuMain();
		}
		break;
	case MENU_PROVIDER:
		{
			ExecuteMenuProvider();
		}
		break;
	case MENU_CATEGORY:
		{
			ExecuteMenuCategory();
		}
		break;
	case MENU_F1:
		{
			ExecuteMenuF1();
		}
		break;
	case MENU_REPORTS:
		{
			ExecuteMenuReports();
		}
		break;
	case MENU_F2:
		{
			ExecuteMenuF2();
		}
		break;
	case MENU_SERVICE_REQUEST:
		{
			ExecuteMenuServiceRequest();
		}
		break;
	case MENU_F3:
		{
			ExecuteMenuF3();
		}
		break;
	case MENU_PHONEBOOK:
		{
			ExecuteMenuPhonebook();
		}
		break;
	case MENU_LANGUAGE:
		{
			ExecuteMenuLanguage();
		}
		break;
	case MENU_POS_SETTINGS:
		{
			ExecuteMenuSettings();
		}
		break;
	case MENU_TMS_SETTINGS:
		{
			ExecuteMenuTMSSettings();
		}
		break;
	case MENU_CONNECTION_TYPE:
		{
			ExecuteMenuConnectionType();
		}
		break;
	}
}

void CChildView::DoCancelMenu(MENU_INFO* pMenuInfo)
{
	switch (pMenuInfo->menuType)
	{
	case MENU_TMS_SETTINGS:
		{
			DoCancelMenuTMSSettings();
		}
		break;
	}
}

void CChildView::PaintViewMenu(MENU_INFO* pMenuInfo, CDCX* pDC)
{
	int		i;
	RECT	rc;

	pDC->SelectObject(&m_fntHeaderLarge);

	if (PaintRequired(PAINT_FULL_SCREEN))
	{
		PaintBackground(pDC);

		GetRectFromPNGResource(&rc, RP_MENU_BOTTOM_LINE);
		pDC->FillSolidRect(&rc, COLOR_RGB_BLACK);

#if MENU_ARROW
		PaintViewMenuArrow(pMenuInfo, pDC);
#endif

		for (i = 0; i < pMenuInfo->nMaxItems; i++)
		{
			PaintViewMenuItem(pMenuInfo, pDC, i);
		}
	}
	else
	{
		if (PaintRequired(PAINT_MENU_ITEM))
		{
			if (pMenuInfo->nPrevPage != pMenuInfo->nCurPage)
			{
				for (i = 0; i < pMenuInfo->nMaxItems; i++)
				{
					PaintViewMenuItem(pMenuInfo, pDC, i);
				}

				if (pMenuInfo->nCurPage == (pMenuInfo->nMaxPages - 1))
				{
					int		nRemains	= (pMenuInfo->nItemsPerPage - (pMenuInfo->nMaxItems - (pMenuInfo->nItemsPerPage * (pMenuInfo->nMaxPages - 1))));
					RECT	rc;

					if (nRemains < pMenuInfo->nItemsPerPage)
					{
						for (i = (pMenuInfo->nItemsPerPage - nRemains); i < pMenuInfo->nItemsPerPage; i++)
						{
#if MENU_ARROW
							GetRectFromPNGResource(&rc, RP_MENU_UNSELECTED[i + (pMenuInfo->bArrow ? 1 : 0)]);
#else
							GetRectFromPNGResource(&rc, RP_MENU_UNSELECTED[i]);
#endif

							pDC->FillSolidRect(&rc, COLOR_RGB_WHITE);
						}
					}
				}
			}
			else
			{
				if (pMenuInfo->nPrevItem != pMenuInfo->nCurItem)
				{
					PaintViewMenuItem(pMenuInfo, pDC, pMenuInfo->nPrevItem);
				}

				PaintViewMenuItem(pMenuInfo, pDC, pMenuInfo->nCurItem);
			}
		}

		if (PaintRequired(PAINT_MENU_ARROW))
		{
			PaintViewMenuArrow(pMenuInfo, pDC);
		}
	}
}

void CChildView::PaintViewMenuArrow(MENU_INFO* pMenuInfo, CDCX* pDC)
{
#if MENU_ARROW
	if (pMenuInfo->bArrow)
	{
		switch (pMenuInfo->menuArrowStatus)
		{
		case MENU_ARROW_UP:
			{
				DrawPNGResource(RP_MENU_UP_SELECTED, pDC->GetSafeHdc());
				DrawPNGResource(RP_MENU_DN_UNSELECTED, pDC->GetSafeHdc());
			}
			break;
		case MENU_ARROW_DOWN:
			{
				DrawPNGResource(RP_MENU_UP_UNSELECTED, pDC->GetSafeHdc());
				DrawPNGResource(RP_MENU_DN_SELECTED, pDC->GetSafeHdc());
			}
			break;
		default:

			{
				DrawPNGResource(RP_MENU_UP_UNSELECTED, pDC->GetSafeHdc());
				DrawPNGResource(RP_MENU_DN_UNSELECTED, pDC->GetSafeHdc());
			}
			break;
		}
	}
#endif
}

void CChildView::PaintViewMenuItem(MENU_INFO* pMenuInfo, CDCX* pDC, int nItem)
{
#if MENU_ARROW
	int		nPos	= (nItem % pMenuInfo->nItemsPerPage) + (pMenuInfo->bArrow ? 1 : 0);
#else
	int		nPos	= (nItem % pMenuInfo->nItemsPerPage);
#endif

	RECT	rc;

	if ((nItem >= (pMenuInfo->nCurPage * pMenuInfo->nItemsPerPage)) && (nItem < ((pMenuInfo->nCurPage + 1) * pMenuInfo->nItemsPerPage))) 
	{
		if (nItem == pMenuInfo->nCurItem)
		{
			DrawPNGResource(RP_MENU_SELECTED[nPos], pDC->GetSafeHdc());
			pDC->SetTextColor(COLOR_RGB_WHITE);
		}
		else
		{
			DrawPNGResource(RP_MENU_UNSELECTED[nPos], pDC->GetSafeHdc());
			pDC->SetTextColor(COLOR_RGB_BLACK);

			if (pMenuInfo->bCheckType)
			{
				if (pMenuInfo->menuItems[nItem].nStatus == MENU_CHECKED)
				{
					DrawPNGResource(RP_MENU_CHECKED[nPos], pDC->GetSafeHdc());
					pDC->SetTextColor(COLOR_RGB_WHITE);
				}
			}
		}

		GetRectFromPNGResource(&rc, RP_MENU_SELECTED[nPos]);

		rc.right	-= 16;
		rc.left		+= 16;

		switch (m_Settings.langMode)
		{
		case LANGUAGE_FARSI:
			{
				pDC->DrawText(pMenuInfo->menuItems[nItem].MenuTitle.szF, &rc, DT_RIGHT | DT_VCENTER);

				if (nItem < 9)
				{
					pDC->DrawText(LoadText(IDS_NUM1 + nItem), &rc, DT_LEFT | DT_VCENTER);
				}
				else
				{
					pDC->DrawText(L"-", &rc, DT_LEFT | DT_VCENTER);
				}
			}
			break;
		default:
			{
				pDC->DrawText(pMenuInfo->menuItems[nItem].MenuTitle.szE, &rc, DT_LEFT | DT_VCENTER);

				if (nItem < 9)
				{
					pDC->DrawText(LoadText(IDS_NUM1 + nItem), &rc, DT_RIGHT | DT_VCENTER);
				}
				else
				{
					pDC->DrawText(L"-", &rc, DT_RIGHT | DT_VCENTER);
				}
			}
			break;
		}
	}
}

void CChildView::OnKeyDownViewMenu(MENU_INFO* pMenuInfo, UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (GetHardwareKey(nChar))
	{
	case HWK_RETURN:
		{
			ExecuteMenu(pMenuInfo);
		}
		break;
	case HWK_ESCAPE:
	case HWK_HOME:
		{
			if (pMenuInfo->bHierarchy)
			{
				DoCancelMenu(pMenuInfo);
				SetViewMode(pMenuInfo->previousViewMode);
				SetTransactionMode(pMenuInfo->previousTransactionMode);
				ForcePaintFlag();
			}
			else
			{
				Restart();
			}
		}
		break;
	case HWK_DOWN:
		{
			pMenuInfo->nPrevItem	= pMenuInfo->nCurItem;
			pMenuInfo->nPrevPage	= pMenuInfo->nCurPage;

			if ((pMenuInfo->nCurItem + 1) < pMenuInfo->nMaxItems)
			{
				pMenuInfo->nCurItem++;
			}
			else
			{
				pMenuInfo->nCurItem	= 0;
			}

			pMenuInfo->nCurPage	= pMenuInfo->nCurItem / pMenuInfo->nItemsPerPage;

			ForcePaintFlag(PAINT_MENU_ITEM);
		}
		break;
	case HWK_UP:
		{
			pMenuInfo->nPrevItem	= pMenuInfo->nCurItem;
			pMenuInfo->nPrevPage	= pMenuInfo->nCurPage;

			if (pMenuInfo->nCurItem > 0)
			{
				pMenuInfo->nCurItem--;
			}
			else
			{
				pMenuInfo->nCurItem	= pMenuInfo->nMaxItems - 1;
			}

			pMenuInfo->nCurPage	= pMenuInfo->nCurItem / pMenuInfo->nItemsPerPage;

			ForcePaintFlag(PAINT_MENU_ITEM);
		}
		break;
	case HWK_RIGHT:
		{
			if (!pMenuInfo->bCheckType)
			{
				return;
			}

			if (pMenuInfo->menuItems[pMenuInfo->nCurItem].nStatus == MENU_UNCHECKED)
			{
				pMenuInfo->menuItems[pMenuInfo->nCurItem].nStatus	= MENU_CHECKED;
			}
			else
			{
				pMenuInfo->menuItems[pMenuInfo->nCurItem].nStatus	= MENU_UNCHECKED;
			}

			ForcePaintFlag(PAINT_MENU_ITEM);
		}
		break;
	default:
		{
			if ((GetHardwareKey(nChar) >= HWK_0) && (GetHardwareKey(nChar) <= HWK_9))
			{
				if ((nChar >= '1') && (nChar < (UINT)('1' + pMenuInfo->nMaxItems)))
				{
					pMenuInfo->nPrevItem	= pMenuInfo->nCurItem;
					pMenuInfo->nPrevPage	= pMenuInfo->nCurPage;
					pMenuInfo->nCurItem		= nChar - '1';
					pMenuInfo->nCurPage		= pMenuInfo->nCurItem / pMenuInfo->nItemsPerPage;

					if (pMenuInfo->bCheckType)
					{
						if (pMenuInfo->menuItems[pMenuInfo->nCurItem].nStatus == MENU_UNCHECKED)
						{
							pMenuInfo->menuItems[pMenuInfo->nCurItem].nStatus	= MENU_CHECKED;
						}
						else
						{
							pMenuInfo->menuItems[pMenuInfo->nCurItem].nStatus	= MENU_UNCHECKED;
						}

						ForcePaintFlag(PAINT_MENU_ITEM);
					}
					else
					{
						ExecuteMenu(pMenuInfo);
					}
				}
			}
		}
		break;
	}
}

void CChildView::OnLButtonDownViewMenu(MENU_INFO* pMenuInfo, UINT nFlags, CPoint point)
{
	RECT	rc;

	for (int i = 0; i < pMenuInfo->nItemsPerPage; i++)
	{
#if MENU_ARROW
		GetRectFromPNGResource(&rc, RP_MENU_UNSELECTED[i + (pMenuInfo->bArrow ? 1 : 0)]);
#else
		GetRectFromPNGResource(&rc, RP_MENU_UNSELECTED[i]);
#endif

		if (PtInRect(&rc, point))
		{
			pMenuInfo->nPrevItem	= pMenuInfo->nCurItem;
			pMenuInfo->nCurItem		= ((pMenuInfo->nCurPage * pMenuInfo->nItemsPerPage) + i);

			ForcePaintFlag(PAINT_MENU_ITEM);
			return;
		}
	}

#if MENU_ARROW
	if (pMenuInfo->bArrow)
	{
		GetRectFromPNGResource(&rc, RP_MENU_UP_UNSELECTED);

		if (PtInRect(&rc, point))
		{
			pMenuInfo->menuArrowStatus	= MENU_ARROW_UP;

			ForcePaintFlag(PAINT_MENU_ARROW);
			return;
		}

		GetRectFromPNGResource(&rc, RP_MENU_DN_UNSELECTED);

		if (PtInRect(&rc, point))
		{
			pMenuInfo->menuArrowStatus	= MENU_ARROW_DOWN;

			ForcePaintFlag(PAINT_MENU_ARROW);
			return;
		}
	}
#endif
}

void CChildView::OnLButtonUpViewMenu(MENU_INFO* pMenuInfo, UINT nFlags, CPoint point)
{
	RECT	rc;

	for (int i = 0; i < pMenuInfo->nItemsPerPage; i++)
	{
#if MENU_ARROW
		GetRectFromPNGResource(&rc, RP_MENU_UNSELECTED[i + (pMenuInfo->bArrow ? 1 : 0)]);
#else
		GetRectFromPNGResource(&rc, RP_MENU_UNSELECTED[i]);
#endif

		if (PtInRect(&rc, point))
		{
			if (pMenuInfo->nCurItem == ((pMenuInfo->nCurPage * pMenuInfo->nItemsPerPage) + i))
			{
				if (pMenuInfo->bCheckType)
				{
					if (pMenuInfo->menuItems[pMenuInfo->nCurItem].nStatus == MENU_UNCHECKED)
					{
						pMenuInfo->menuItems[pMenuInfo->nCurItem].nStatus	= MENU_CHECKED;
					}
					else
					{
						pMenuInfo->menuItems[pMenuInfo->nCurItem].nStatus	= MENU_UNCHECKED;
					}

					ForcePaintFlag(PAINT_MENU_ITEM);
				}
				else
				{
					ExecuteMenu(pMenuInfo);
				}
			}
			else
			{
				ForcePaintFlag(PAINT_MENU_ITEM);
			}

			return;
		}
	}

#if MENU_ARROW
	if (pMenuInfo->bArrow)
	{
		GetRectFromPNGResource(&rc, RP_MENU_UP_UNSELECTED);

		if (PtInRect(&rc, point))
		{
			pMenuInfo->nPrevItem	= pMenuInfo->nCurItem;
			pMenuInfo->nPrevPage	= pMenuInfo->nCurPage;

			if (pMenuInfo->nCurItem > 0)
			{
				pMenuInfo->nCurItem--;
			}
			else
			{
				pMenuInfo->nCurItem	= pMenuInfo->nMaxItems - 1;
			}

			pMenuInfo->nCurPage			= pMenuInfo->nCurItem / pMenuInfo->nItemsPerPage;
			pMenuInfo->menuArrowStatus	= MENU_ARROW_NONE;

			ForcePaintFlag(PAINT_MENU_ITEM | PAINT_MENU_ARROW);
			return;
		}

		GetRectFromPNGResource(&rc, RP_MENU_DN_UNSELECTED);

		if (PtInRect(&rc, point))
		{
			pMenuInfo->nPrevItem	= pMenuInfo->nCurItem;
			pMenuInfo->nPrevPage	= pMenuInfo->nCurPage;

			if ((pMenuInfo->nCurItem + 1) < pMenuInfo->nMaxItems)
			{
				pMenuInfo->nCurItem++;
			}
			else
			{
				pMenuInfo->nCurItem	= 0;
			}

			pMenuInfo->nCurPage			= pMenuInfo->nCurItem / pMenuInfo->nItemsPerPage;
			pMenuInfo->menuArrowStatus	= MENU_ARROW_NONE;

			ForcePaintFlag(PAINT_MENU_ITEM | PAINT_MENU_ARROW);
			return;
		}
	}
#endif
}
