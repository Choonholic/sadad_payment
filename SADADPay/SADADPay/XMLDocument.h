// XMLDocument.h

#pragma once

#include <objsafe.h>
#include <ocidl.h>
#include <msxml2.h>

#define XmlDocument	IXMLDOMDocument* 
#define XmlNode		IXMLDOMNode*
#define XmlNodeList	IXMLDOMNodeList*
#define XmlElement	IXMLDOMElement*

class CXMLDocument
{
public:
	CXMLDocument();
	~CXMLDocument();

	void	Close();

	BOOL	Load(CComVariant varFile);
	BOOL	LoadXML(CString* strXML);
	BOOL	Save(CComVariant varFile);

	HRESULT	SelectSingleNode(CString* strPath, XmlNode* pNode);
	HRESULT	SelectNodes(CString* strPath, XmlNodeList* pNodeList);

	CString	GetNodeText(CString* strPath);
	CString	GetNodeText(CString* strPath, CString* strChildNodeName);
	CString	GetNodeText(XmlNode* pNode, CString* strChildNodeName);

	HRESULT	SetNodeText(CString* strPath, CString* strValue);
	HRESULT	SetNodeText(CString* strPath, CString* strChildNodeName, CString* strValue);
	HRESULT	SetNodeText(XmlNode* pNode, CString* strChildNodeName, CString* strValue);

	HRESULT	GetDocumentElement(XmlElement* pElement);

	HRESULT	AppendChild(XmlNode* pNode, CString* strNodeName, CString* strNodeValue);
	HRESULT RemoveChild(XmlNode* pNode);

	XmlDocument	m_oXMLdoc;
};
