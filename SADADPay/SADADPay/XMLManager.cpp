// XMLManager.cpp

#include "StdAfx.h"
#include "XMLManager.h"
#include <objsafe.h>
#include <objbase.h>

CXMLManager::CXMLManager() : m_iXmlDoc(NULL), m_bInitialized(FALSE), m_bReleased(FALSE)
{
}

CXMLManager::~CXMLManager()
{
	if (m_bReleased == FALSE)
	{
		this->Release();
	}
}

BOOL CXMLManager::Init()
{
	if (FAILED(::CoInitializeEx(NULL, COINIT_MULTITHREADED)))
	{
		return FALSE;
	}

	if (FAILED(m_iXmlDoc.CoCreateInstance(__uuidof(DOMDocument))))
	{
		return FALSE;
	}

	m_iXmlDoc->put_async(VARIANT_FALSE);

	m_bInitialized	= TRUE;
	
	return TRUE;
}

void CXMLManager::Release()
{
	if (m_iXmlDoc != NULL)
	{
		m_iXmlDoc.Release();

		m_iXmlDoc	= NULL;
	}

	CoUninitialize();

	m_bReleased	= TRUE;
}

void CXMLManager::CheckInit()
{
	if (m_bInitialized != TRUE)
	{
		Init();
	}
}

BOOL CXMLManager::Load(CString strFileName)
{
	VARIANT_BOOL	bLoadResult = FALSE;

	CheckInit();

	if (FAILED(m_iXmlDoc->load(CComVariant(strFileName), &bLoadResult)))
	{
		return FALSE;
	}

	return (BOOL)(bLoadResult == VARIANT_TRUE);
}

#define ERROR_MESSAGE_FORMAT	L"CXMLManager::GetParseError() [0x%.8X] on line %d, position %d\r\nReason: %s\r\n"

void CXMLManager::GetParseError()
{
	HRESULT				hResult		= E_FAIL;
	long				line		= 0;
	long				linePos		= 0;
	BSTR				reason		= NULL;
	IXMLDOMParseError*	parseError	= NULL;

	m_iXmlDoc->get_parseError(&parseError);

	parseError->get_errorCode(&hResult);
	parseError->get_line(&line);
	parseError->get_linepos(&linePos);
	parseError->get_reason(&reason);
	
	RETAILMSG(1, (ERROR_MESSAGE_FORMAT, hResult, line, linePos, CString(reason)));

	parseError->Release();
	SysFreeString(reason);
}

HRESULT CXMLManager::SetNodeText(CString strNodePath, CString strText)
{
	HRESULT			hResult	= E_FAIL;
	IXMLDOMNode*	pNode	= FindNodeByPath(strNodePath);

	if (pNode == NULL)
	{
		return E_FAIL;
	}

	hResult	= pNode->put_text((TCHAR*)(LPCTSTR)strText);

	pNode->Release();

	return hResult;
}

HRESULT CXMLManager::SetNodeText(CString strNodePath, CString strChildName, CString strText)
{
	IXMLDOMNode*	pNode	= FindNodeByPath(strNodePath);

	if (pNode == NULL)
	{
		return E_FAIL;
	}

	HRESULT	hResult	= SetNodeText(&pNode, strChildName, strText);

	SAFE_RELEASE(pNode);

	return hResult;
}

HRESULT	CXMLManager::SetNodeText(IXMLDOMNode **pNode, CString strText)
{
	return ((*pNode)->put_text((TCHAR*)(LPCTSTR)strText));
}

HRESULT CXMLManager::SetNodeText(IXMLDOMNode **pNode, CString strChildName, CString strText)
{
	HRESULT				hResult		= E_FAIL;
	IXMLDOMNodeList*	pNodeList	= NULL;
	long				lLength		= 0;

	(*pNode)->get_childNodes(&pNodeList);
	pNodeList->get_length(&lLength);

	for (int i = 0; i < (int)lLength; i++)
	{
		IXMLDOMNode*	pItem		= NULL;
		BSTR			nodeName	= NULL;
		CString			strNodeName	= L"";

		pNodeList->get_item(i, &pItem);
		pItem->get_nodeName(&nodeName);
		strNodeName.Format(L"%s", nodeName);
		SysFreeString(nodeName);

		if (strChildName.Compare(strNodeName) == 0)
		{
			hResult	= pItem->put_text((TCHAR*)(LPCTSTR)strText);
			i		= lLength + 1;
		}

		SAFE_RELEASE(pItem);
	}

	SAFE_RELEASE(pNodeList);

	return hResult;
}

CString CXMLManager::GetNodeText(IXMLDOMNode* pNode)
{
	BSTR	text	= NULL;
	CString	strText	= L"";

	if (!FAILED(pNode->get_text(&text)))
	{
		strText.Format(L"%s", text);
	}

	SysFreeString(text);

	return strText;
}

CString CXMLManager::GetNodeText(IXMLDOMNode* pNode, CString strChildName)
{
	IXMLDOMNode*	pChild	= this->FindNode(pNode, strChildName, FALSE);

	if (pChild == NULL)
	{
		return L"";
	}

	CString	strText	= GetNodeText(pChild);

	SAFE_RELEASE(pChild);

	return strText;
}

CString CXMLManager::GetNodeText(CString strNodePath)
{
	IXMLDOMNode*	pNode	= FindNodeByPath(strNodePath);

	if (pNode == NULL)
	{
		return L"";
	}

	CString	strText	= GetNodeText(pNode);

	SAFE_RELEASE(pNode);

	return strText;
}

HRESULT	CXMLManager::Save(CString strFileName)
{
	return (m_iXmlDoc->save(CComVariant(strFileName)));
}

HRESULT CXMLManager::GetXML(BSTR* pBstr)
{
	return m_iXmlDoc->get_xml(pBstr);
}

CString CXMLManager::GetNodeName(IXMLDOMNode* pNode)
{
	BSTR	name	= NULL;
	CString	strName	= L"";

	if (pNode->get_nodeName(&name) == S_OK)
	{
		strName.Format(L"%s", name);
	}

	SysFreeString(name);

	return strName;
}

IXMLDOMNode* CXMLManager::FindNode(IXMLDOMNode* pParent, CString strFindName, BOOL bChildFind)
{
	IXMLDOMNode*	pNext		= NULL;
	IXMLDOMNode*	pChild		= NULL;
	BSTR			nodeName	= NULL;
	
	if (pParent == NULL)
	{
		return NULL;
	}
		
	pParent->get_firstChild(&pChild);

	if (pChild == NULL)
	{
		return NULL;
	}

	do 
	{
		pChild->get_nextSibling(&pNext);
		pChild->get_nodeName(&nodeName);

		if (strFindName.Compare(nodeName) == 0)
		{
			SysFreeString(nodeName);
			SAFE_RELEASE(pNext);
		
			return pChild;
		}

		if (bChildFind == TRUE)
		{
			IXMLDOMNode*	pNode	= FindNode(pChild, strFindName, bChildFind);

			if (pNode != NULL)
			{
				SysFreeString(nodeName);
				SAFE_RELEASE(pChild);
				SAFE_RELEASE(pNext);
			
				return pNode;
			}

			pNode->Release();
		}

		SAFE_RELEASE(pChild);

		pChild = pNext;
	} while (pChild != NULL);

	SysFreeString(nodeName);
	SAFE_RELEASE(pChild);
	SAFE_RELEASE(pNext);

	return NULL;
}

IXMLDOMNode* CXMLManager::FindChildNode(IXMLDOMNode* pParentNode, CString strChildName)
{
	IXMLDOMNodeList*	pNodeList	= NULL;
	long				lLength		= 0;

	pParentNode->get_childNodes(&pNodeList);
	pNodeList->get_length(&lLength);

	for (int i = 0; i < (int)lLength; i++)
	{
		IXMLDOMNode*	pItem	= NULL;

		pNodeList->get_item(i, &pItem);		

		CString	strNodeName	= this->GetNodeName(pItem);

		if (strChildName.Compare(strNodeName) == 0)
		{
			SAFE_RELEASE(pNodeList);

			return pItem;
		}

		SAFE_RELEASE(pItem);
	}

	SAFE_RELEASE(pNodeList);

	return NULL;
}

IXMLDOMNode* CXMLManager::FindNodeByName(CString strFindName)
{
	IXMLDOMNode*	pRootNode	= GetRootNode();

	if (pRootNode == NULL)
	{
		return NULL;
	}

	IXMLDOMNode*	pFindNode	= FindNode(pRootNode, strFindName, TRUE);

	SAFE_RELEASE(pRootNode);

	return pFindNode;
}

IXMLDOMNode* CXMLManager::FindNodeByPath(CString strNodePath)
{
	IXMLDOMNode*	pNode	= NULL;

	if (FAILED(this->SelectSingleNode(strNodePath, &pNode)))
	{
		return NULL;
	}

	return pNode;
}

IXMLDOMNode* CXMLManager::GetRootNode()
{
	IXMLDOMElement*	pRoot	= NULL;

	if (FAILED(m_iXmlDoc->get_documentElement(&pRoot)))
	{
		return NULL;
	}

	return (IXMLDOMNode*)pRoot;
}

HRESULT CXMLManager::SelectNodes(CString strNodePath, IXMLDOMNodeList** pResultList)
{
	return (m_iXmlDoc->selectNodes((TCHAR*)(LPCTSTR)strNodePath, pResultList));
}

HRESULT CXMLManager::SelectSingleNode(CString strNodePath, IXMLDOMNode** pResultNode)
{
	return (m_iXmlDoc->selectSingleNode((TCHAR*)(LPCTSTR)strNodePath, pResultNode));
}

HRESULT CXMLManager::CreateElement(CString strNewElementName, IXMLDOMElement* pInsertBefore)
{
	IXMLDOMElement*	pNewElement	= NULL;
	IXMLDOMNode*	pParentNode	= NULL;

	if (FAILED(m_iXmlDoc->createElement((TCHAR*)(LPCTSTR)strNewElementName, &pNewElement)))
	{
		return E_FAIL;
	}

	if (FAILED(pInsertBefore->get_parentNode(&pParentNode)))
	{
		return E_FAIL;
	}

	HRESULT	hResult	= pParentNode->insertBefore((IXMLDOMNode*)pNewElement, CComVariant(pInsertBefore), (IXMLDOMNode**)&pNewElement);

	SAFE_RELEASE(pNewElement);
	SAFE_RELEASE(pParentNode);

	return hResult;
}

HRESULT CXMLManager::CreateElement(CString strNewElementName, CString strParentPath)
{
	IXMLDOMElement*	pNewElement	= NULL;
	IXMLDOMNode*	pParentNode	= NULL;

	if (FAILED(m_iXmlDoc->createElement((TCHAR*)(LPCTSTR)strNewElementName, &pNewElement)))
	{
		return E_FAIL;
	}

	if (FAILED(this->SelectSingleNode(strParentPath, &pParentNode)))
	{
		return E_FAIL;
	}
	else if (pParentNode == NULL)
	{
		return E_FAIL;
	}

	HRESULT	hResult	= pParentNode->appendChild((IXMLDOMNode*)pNewElement, (IXMLDOMNode**)&pNewElement);

	SAFE_RELEASE(pNewElement);
	SAFE_RELEASE(pParentNode);

	return hResult;
}

HRESULT CXMLManager::CreateElement(CString strNewElementName, IXMLDOMNode** pParentNode)
{
	IXMLDOMElement*	pNewElement	= NULL;

	if (FAILED(m_iXmlDoc->createElement((TCHAR*)(LPCTSTR)strNewElementName, &pNewElement)))
	{
		return E_FAIL;
	}

	HRESULT	hResult	= (*pParentNode)->appendChild((IXMLDOMNode*)pNewElement, (IXMLDOMNode**)&pNewElement);

	SAFE_RELEASE(pNewElement);

	return hResult;
}

HRESULT CXMLManager::RemoveNode(CString strPath)
{
	IXMLDOMNode*	pNode		= NULL;
	IXMLDOMNode*	pParentNode	= NULL;

	this->SelectSingleNode(strPath, &pNode);

	if (FAILED(pNode->get_parentNode(&pParentNode)))
	{
		return E_FAIL;
	}

	HRESULT	hResult	= RemoveChild(pParentNode, pNode);
	
	SAFE_RELEASE(pNode);
	SAFE_RELEASE(pParentNode);

	return hResult;
}	

HRESULT CXMLManager::RemoveChild(CString strParentPath, CString strNodeName)
{
	HRESULT			hResult		= E_FAIL;
	IXMLDOMNode*	pParentNode	= NULL;

	if (FAILED(m_iXmlDoc->selectSingleNode((TCHAR*)(LPCTSTR)strParentPath, &pParentNode)))
	{
		return E_FAIL;
	}

	IXMLDOMNode*	pChildNode	= FindChildNode(pParentNode, strNodeName);

	if (pChildNode != NULL)
	{
		hResult	= RemoveChild(pParentNode, pChildNode);
	}

	SAFE_RELEASE(pChildNode);
	SAFE_RELEASE(pParentNode);

	return hResult;
}	

HRESULT CXMLManager::RemoveChild(IXMLDOMNode* pParentNode, IXMLDOMNode* pRemoveNode)
{
	IXMLDOMNode*	pNode	= NULL;
	HRESULT			hResult	= pParentNode->removeChild(pRemoveNode, &pNode);

	if ((pNode == NULL) && (hResult != E_INVALIDARG))
	{	
		return E_FAIL;
	}

	SAFE_RELEASE(pNode);

	return hResult;
}	

BOOL CXMLManager::IsExistChildNode(IXMLDOMNode* pParentNode, CString strChildName)
{
	PXmlNodeList	pNodeList	= NULL;
	long			lLength		= 0;

	pParentNode->get_childNodes(&pNodeList);
	pNodeList->get_length(&lLength);

	if (!strChildName.IsEmpty())
	{
		int	nFind	= 0;

		for (int i = 0; i < lLength; i++)
		{
			PXmlNode	pItem	= NULL;

			pNodeList->get_item(i, &pItem);

			CString		strName	= this->GetNodeName(pItem);

			if (strName.Compare(strChildName) == 0)
			{
				nFind	= i;
				i		= lLength + 1;
			}

			pItem->Release();
		}

		lLength	= nFind;
	}

	return (BOOL)(lLength > 0);
}

HRESULT CXMLManager::AppendChild(CString strParentPath, IXMLDOMNode** pInsertChild)
{
	IXMLDOMNode*	pParentNode	= NULL;

	if ((pInsertChild == NULL) || (strParentPath.IsEmpty()))
	{
		return E_FAIL;
	}

	if (FAILED(SelectSingleNode(strParentPath, &pParentNode)))
	{
		return E_FAIL;
	}

	HRESULT	hResult	= pParentNode->appendChild((*pInsertChild), pInsertChild);

	SAFE_RELEASE(pParentNode);

	return hResult;
}

HRESULT CXMLManager::AppendChild(IXMLDOMNode** pParentNode, IXMLDOMNode** pInsertChild)
{
	if ((pInsertChild == NULL) || (pParentNode == NULL))
	{
		return E_FAIL;
	}

	return (*pParentNode)->appendChild((*pInsertChild), pInsertChild);;
}

HRESULT CXMLManager::SetAttribute(IXMLDOMElement* pElement, CString strAttrName, CString strValue)
{
	return (pElement->setAttribute(strAttrName.AllocSysString(), CComVariant(strValue)));
}

HRESULT CXMLManager::RemoveAttribute(IXMLDOMElement* pElement, CString strAttrName)
{
	return (pElement->removeAttribute(strAttrName.AllocSysString()));
}

CString CXMLManager::GetAttributeData(IXMLDOMNode* pNode, CString strAttrName)
{
	CString					strAttr		= L"";
	IXMLDOMNamedNodeMap*	pAttrMap	= NULL;
	IXMLDOMElement*			pElement	= (IXMLDOMElement*)pNode;
	long					lLength		= 0;

	pElement->get_attributes(&pAttrMap);
	pAttrMap->get_length(&lLength);

	for (int i = 0; i < lLength; i++)
	{
		IXMLDOMNode*	pItem		= NULL;
		HRESULT			hResult		= pAttrMap->get_item(i, &pItem);
		CString			strNodeName = this->GetNodeName(pItem);

		if (strNodeName.Compare(strAttrName) == 0)
		{
			strAttr	= this->GetNodeText(pItem);
			i		= lLength;
		}

		SAFE_RELEASE(pItem);
	}
	
	pElement	= NULL;

	SAFE_RELEASE(pAttrMap);

	return strAttr;
}