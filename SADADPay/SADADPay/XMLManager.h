// XMLManager.h

#pragma once

#include <windows.h>
#include <msxml2.h>

#define	SAFE_RELEASE(pNode)		{ if (pNode != NULL) pNode->Release(); pNode = NULL; }

#define	PXmlDocument	IXMLDOMDocument* 
#define	PXmlNode		IXMLDOMNode*
#define	PXmlNodeList	IXMLDOMNodeList*
#define	PXmlElement		IXMLDOMElement*

#define	XmlDocument		IXMLDOMDocument
#define	XmlNode			IXMLDOMNode
#define	XmlNodeList		IXMLDOMNodeList
#define	XmlElement		IXMLDOMElement

class CXMLManager
{
public:
	CXMLManager();
	~CXMLManager();

	BOOL	Init();
	void	Release();

	BOOL	Load(CString strFileName);

	HRESULT	SetNodeText(CString strNodePath, CString strText);
	HRESULT	SetNodeText(CString strNodePath, CString strChildName, CString strText);
	HRESULT	SetNodeText(IXMLDOMNode** pNode, CString strText);
	HRESULT	SetNodeText(IXMLDOMNode** pNode, CString strChildName, CString strText);

	CString GetNodeText(IXMLDOMNode* pNode);
	CString GetNodeText(IXMLDOMNode* pNode, CString strChildName);
	CString GetNodeText(CString strNodePath);

	HRESULT Save(CString strFileName);

	HRESULT GetXML(BSTR *pBstr);

	CString GetNodeName(IXMLDOMNode* pNode);

	IXMLDOMNode*	FindNode(IXMLDOMNode* pParent, CString strFindName, BOOL bChildFind);
	IXMLDOMNode*	FindChildNode(IXMLDOMNode *pParentNode, CString strChildName);
	IXMLDOMNode*	FindNodeByName(CString strFindName);
	IXMLDOMNode*	FindNodeByPath(CString strNodePath);

	IXMLDOMNode*	GetRootNode();
	HRESULT			SelectNodes(CString strNodePath, IXMLDOMNodeList** pResultList);
	HRESULT			SelectSingleNode(CString strNodePath, IXMLDOMNode** pResultNode);

	HRESULT	CreateElement(CString strNewElementName, IXMLDOMElement* pInsertBefore);
	HRESULT	CreateElement(CString strNewElementName, CString strParentPath);
	HRESULT	CreateElement(CString strNewElementName, IXMLDOMNode** pParentNode);

	HRESULT RemoveNode(CString strPath);

	HRESULT RemoveChild(CString strParentPath, CString strNodeName);
	HRESULT RemoveChild(IXMLDOMNode* pParentNode, IXMLDOMNode* pRemoveNode);

	BOOL	IsExistChildNode(IXMLDOMNode* pParentNode, CString strChildName = L"");

	HRESULT	AppendChild(CString strParentPath, IXMLDOMNode** pInsertChild);
	HRESULT	AppendChild(IXMLDOMNode** pParentNode, IXMLDOMNode** pInsertChild);

	HRESULT	SetAttribute(IXMLDOMElement* pElement, CString strAttrName, CString strValue);
	HRESULT	RemoveAttribute(IXMLDOMElement* pElement, CString strAttrName);
	CString	GetAttributeData(IXMLDOMNode* pNode, CString strAttrName);

private:
	void	CheckInit();

	void	GetParseError();

	CComPtr<IXMLDOMDocument>	m_iXmlDoc;

	BOOL	m_bInitialized;
	BOOL	m_bReleased;
};
