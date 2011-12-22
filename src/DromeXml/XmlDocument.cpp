/*
 * Copyright (C) 2010-2011 Josh A. Beam
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *   1. Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <fstream>
#include <DromeXml/XmlDocument.h>
#include <DromeXml/XmlException.h>
#include "XmlUtil.h"

using namespace std;

namespace DromeXml {

XmlDocument::XmlDocument()
{
	m_rootElement = NULL;
	m_currentElement = NULL;
}

XmlDocument::~XmlDocument()
{
	if(m_rootElement != NULL)
		delete m_rootElement;
}

void
XmlDocument::parsedProcessingInstruction(const string &/*target*/)
{
}

void
XmlDocument::parsedStartTag(const string &tagName)
{
	XmlElement *element = new XmlElement(m_currentElement, tagName.c_str());

	// if we don't have a root element yet, this must be the root;
	// otherwise add it as a child to the current element
	if(!m_rootElement)
		m_rootElement = element;
	else
		m_currentElement->addChild(element);

	m_currentElement = element;
}

void
XmlDocument::parsedEndTag(const string &tagName)
{
	if(!m_currentElement)
		throw XmlException(m_line, "XmlDocument::parsedEndTag(): Got end tag with no current element");

	if(tagName.length() != 0 && tagName.compare(m_currentElement->getName()) != 0)
		throw XmlException(m_line, "XmlDocument::parsedEndTag(): Got end tag with invalid name");

	// end tag is valid, so we're done with the
	// current element and can go back to the parent
	m_currentElement = m_currentElement->getParent();
}

void
XmlDocument::parsedAttribute(const string &name, const string &value)
{
	if(!m_currentElement) {
		//throw XmlException("XmlDocument::parsedAttribute(): Got attribute with no current element");
		return;
	}

	XmlAttribute *attribute = new XmlAttribute(name.c_str(), decodeString(value).c_str());
	m_currentElement->addAttribute(attribute);
}

void
XmlDocument::parsedContent(const string &content)
{
	if(!m_currentElement)
		throw XmlException(m_line, "XmlDocument::parsedContent(): Got content with no current element");

	XmlElement *element = new XmlElement(m_currentElement);
	element->setContent(content.c_str());

	m_currentElement->addChild(element);
}

void
XmlDocument::fromFile(const char *filePath)
{
	if(m_rootElement != NULL)
		delete m_rootElement;
	m_rootElement = NULL;
	m_currentElement = NULL;

	// open file for reading
	ifstream in(filePath);
	if(in.is_open() == false)
		throw XmlException("XmlDocument::fromFile(): Unable to open file for reading");

	// read and process each line
	while(in.eof() == false) {
		string line;
		getline(in, line);
		line += '\n';
		processData(line.c_str());
	}

	in.close();
}

void
XmlDocument::fromFile(const std::string &filePath)
{
	fromFile(filePath.c_str());
}

void
XmlDocument::fromString(const char *s)
{
	if(m_rootElement != NULL)
		delete m_rootElement;
	m_rootElement = NULL;
	m_currentElement = NULL;

	processData(s);
}

void
XmlDocument::fromString(const std::string &s)
{
	fromString(s.c_str());
}

const XmlElement *
XmlDocument::getRootElement() const
{
	return m_rootElement;
}

void
XmlDocument::setRootElement(XmlElement *value)
{
	m_rootElement = value;
}

string
XmlDocument::toString() const
{
	if(!m_rootElement)
		return string("");

	return string("<?xml version=\"1.0\"?>\n") + m_rootElement->toString();
}

} // namespace DromeXml
