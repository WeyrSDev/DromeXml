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

#include <DromeXml/XmlElement.h>
#include <DromeXml/XmlException.h>
#include "XmlUtil.h"

using namespace std;

namespace DromeXml {

XmlElement::XmlElement(XmlElement *parent, const char *name)
{
	m_parent = parent;
	m_name = name;
}

XmlElement::~XmlElement()
{
	// delete attributes
	for(unsigned int i = 0; i < m_attributes.size(); ++i)
		delete m_attributes[i];

	// delete children
	for(unsigned int i = 0; i < m_children.size(); ++i)
		delete m_children[i];
}

XmlElement *
XmlElement::getParent()
{
	return m_parent;
}

string
XmlElement::getName() const
{
	return m_name;
}

void
XmlElement::setName(const char *name)
{
	m_name = name;
}

string
XmlElement::getContent() const
{
	return m_content;
}

void
XmlElement::setContent(const char *content)
{
	m_content = content;
}

unsigned int
XmlElement::getNumAttributes() const
{
	return (unsigned int)m_attributes.size();
}

const XmlAttribute *
XmlElement::getAttribute(unsigned int index) const
{
	if(index >= m_attributes.size())
		throw XmlException("XmlElement::getAttribute(): Invalid index");

	return m_attributes[index];
}

const XmlAttribute *
XmlElement::getAttribute(const string &name) const
{
	for(unsigned int i = 0; i < m_attributes.size(); ++i) {
		if(name == m_attributes[i]->getName())
			return m_attributes[i];
	}

	return NULL;
}

const XmlAttribute *
XmlElement::getAttribute(const char *name) const
{
	return getAttribute(string(name));
}

void
XmlElement::addAttribute(XmlAttribute *attribute)
{
	m_attributes.insert(m_attributes.end(), attribute);
}

unsigned int
XmlElement::getNumChildren() const
{
	return (unsigned int)m_children.size();
}

const XmlElement *
XmlElement::getChild(unsigned int index) const
{
	if(index >= m_children.size())
		throw XmlException("XmlElement::getChild(): Invalid index");

	return m_children[index];
}

const XmlElement *
XmlElement::getChild(const string &name) const
{
	for(unsigned int i = 0; i < m_children.size(); ++i) {
		if(name == m_children[i]->getName())
			return m_children[i];
	}

	return NULL;
}

const XmlElement *
XmlElement::getChild(const char *name) const
{
	return getChild(string(name));
}

void
XmlElement::addChild(XmlElement *child)
{
	m_children.insert(m_children.end(), child);
}

string
XmlElement::toString(int tabLevel) const
{
	string str;

	for(int i = 0; i < tabLevel; ++i)
		str += '\t';

	// if the element has no name, it's a text data
	// element and its content can just be returned
	if(m_name.length() == 0) {
		str += encodeString(m_content);
		return str;
	}

	str += "<" + m_name;

	// append attributes to string
	for(unsigned int i = 0; i < m_attributes.size(); ++i)
		str += " " + m_attributes[i]->toString();

	// append children to string
	if(m_children.size() != 0) {
		str += ">";

		for(unsigned int i = 0; i < m_children.size(); ++i)
			str += '\n' + m_children[i]->toString(tabLevel + 1);

		str += '\n';
		for(int i = 0; i < tabLevel; ++i)
			str += '\t';
		str += "</" + m_name + ">";
	} else {
		str += " />";
	}

	return str;
}

} // namespace DromeXml
