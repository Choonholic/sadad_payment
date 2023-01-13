// XMLDocument.cpp

#include "StdAfx.h"
#include "SADADPay.h"
#include "XMLDocument.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CXMLDocument::CXMLDocument()
{
}

CXMLDocument::~CXMLDocument()
{
}

void CXMLDocument::Close()
{
    m_oXMLdoc->Release();
}

BOOL CXMLDocument::Load(CComVariant varFile)
{
	HRESULT	hr;

	m_oXMLdoc	= NULL;
	hr			= CoInitializeEx(NULL,COINIT_MULTITHREADED);
	hr			= CoCreateInstance(CLSID_DOMDocument, NULL,CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER, IID_IXMLDOMDocument, (LPVOID*)&m_oXMLdoc);
    
	if (m_oXMLdoc)
	{
		VARIANT_BOOL	vSuccess;

		m_oXMLdoc->put_async(VARIANT_FALSE);

		hr	= m_oXMLdoc->load(varFile, &vSuccess);

		return (BOOL)(hr != S_FALSE);
    }

	return FALSE;
}

BOOL CXMLDocument::LoadXML(CString* strXML)
{
	HRESULT	hr;

	m_oXMLdoc	= NULL;
	hr			= CoInitializeEx(NULL,COINIT_MULTITHREADED);
	hr			= CoCreateInstance(CLSID_DOMDocument, NULL,CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER, IID_IXMLDOMDocument, (LPVOID *)&m_oXMLdoc);

	if (m_oXMLdoc)
	{
		VARIANT_BOOL	vSuccess;

		m_oXMLdoc->put_async(VARIANT_FALSE);

		hr	= m_oXMLdoc->loadXML((BSTR)strXML->GetString(), &vSuccess);
        
		return (BOOL)(hr != S_FALSE);
    }

	return FALSE;
}

BOOL CXMLDocument::Save(CComVariant varFile)
{
	HRESULT	hr;

	m_oXMLdoc->put_preserveWhiteSpace(FALSE);

	hr	= m_oXMLdoc->save(varFile);

	return (BOOL)(hr != S_FALSE);
}

HRESULT CXMLDocument::SelectSingleNode(CString* strPath, XmlNode* pNode)
{
	return m_oXMLdoc->selectSingleNode((BSTR)strPath->GetString(), pNode);
}

HRESULT CXMLDocument::SelectNodes(CString* strPath, XmlNodeList* pNodeList)
{
	return m_oXMLdoc->selectNodes((BSTR)strPath->GetString(), pNodeList);
}

CString CXMLDocument::GetNodeText(CString* strPath)
{
	BSTR	bStr;
	CString	strValue = L"";
	XmlNode	node;

	m_oXMLdoc->selectSingleNode((BSTR)strPath->GetString(), &node);
	node->get_text(&bStr);
	strValue.SetString(bStr);

	node->Release();
	SysFreeString(bStr);

	return strValue;
}

CString CXMLDocument::GetNodeText(CString* strPath, CString* strChildNodeName)
{
    BSTR	bStr;
	CString	strValue;
	XmlNode	node;
	XmlNode subNode;

	m_oXMLdoc->selectSingleNode((BSTR)strPath->GetString(), &node);

	node->selectSingleNode((BSTR)strChildNodeName->GetString(), &subNode);
	subNode->get_text(&bStr);
	strValue.SetString(bStr);

	node->Release();
	subNode->Release();
	SysFreeString(bStr);

	return bStr;
}

CString CXMLDocument::GetNodeText(XmlNode* pNode, CString* strChildNodeName)
{
	XmlNode	subNode;
	HRESULT	hr;
	BSTR	bstrValue;
	CString	strValue;

	hr	= (*pNode)->selectSingleNode((BSTR)strChildNodeName->GetString(), &subNode);

	subNode->get_text(&bstrValue);
	strValue.SetString(bstrValue);

	subNode->Release();
	SysFreeString(bstrValue);

	return strValue;
}

HRESULT CXMLDocument::SetNodeText(CString* strPath, CString* strValue)
{
	HRESULT	hr;
	XmlNode node;

	m_oXMLdoc->selectSingleNode((BSTR)strPath->GetString(), &node);

	hr	= node->put_text((BSTR)strValue->GetString());

	node->Release();

	return hr;
}

HRESULT CXMLDocument::SetNodeText(CString* strPath, CString* strChildNodeName, CString* strValue)
{
	HRESULT hr;
	XmlNode node;
	XmlNode subNode;

    m_oXMLdoc->selectSingleNode((BSTR)strPath->GetString(), &node);
	node->selectSingleNode((BSTR)strChildNodeName->GetString(), &subNode);

	hr	= subNode->put_text((BSTR)strValue->GetString());

	subNode->Release();
	node->Release();

	return hr;
}

HRESULT CXMLDocument::SetNodeText(XmlNode* pNode, CString* strChildNodeName, CString* strValue)
{
	HRESULT	hr;
	XmlNode	subNode;

	(*pNode)->selectSingleNode((BSTR)strChildNodeName->GetString(), &subNode);

	hr	= subNode->put_text((BSTR)strValue->GetString());

	subNode->Release();

	return hr;
}

HRESULT CXMLDocument::GetDocumentElement(XmlElement* pElement)
{
	return m_oXMLdoc->get_documentElement(pElement);
}

HRESULT CXMLDocument::AppendChild(XmlNode* pNode, CString* strNodeName, CString* strNodeValue)
{
	HRESULT		hr;
	XmlNode		newNode;
	XmlNode		newNodeOut;
	CComVariant	varNodeType	= NODE_ELEMENT;

	m_oXMLdoc->createNode(varNodeType, (BSTR)strNodeName->GetString(), L"", &newNode);
	newNode->put_text((BSTR)strNodeValue->GetString());

	hr	= (*pNode)->appendChild(newNode, &newNodeOut);

	newNodeOut->Release();
    newNode->Release();

	return hr;
}

HRESULT CXMLDocument::RemoveChild(XmlNode* node)
{
	HRESULT		hr;
	XmlElement	element;
	XmlNode		oldNode;

	GetDocumentElement(&element);

	hr	= element->removeChild(*node, &oldNode);

	oldNode->Release();
	element->Release();

	return hr;
}
