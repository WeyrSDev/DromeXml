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

#ifndef __DROMEXML_XMLELEMENT_H__
#define __DROMEXML_XMLELEMENT_H__

#include <vector>
#include "XmlAttribute.h"

namespace DromeXml {

class XmlElement
{
	protected:
		XmlElement *m_parent;
		std::string m_name;
		std::string m_content;
		std::vector <XmlAttribute *> m_attributes;
		std::vector <XmlElement *> m_children;

	public:
		XmlElement(XmlElement *parent, const char *name = "");
		virtual ~XmlElement();

		XmlElement *getParent();

		std::string getName() const;
		void setName(const char *name);

		std::string getContent() const;
		void setContent(const char *content);

		unsigned int getNumAttributes() const;
		const XmlAttribute *getAttribute(unsigned int index) const;
		const XmlAttribute *getAttribute(const std::string &name) const;
		const XmlAttribute *getAttribute(const char *name) const;
		void addAttribute(XmlAttribute *attribute);

		unsigned int getNumChildren() const;
		const XmlElement *getChild(unsigned int index) const;
		const XmlElement *getChild(const std::string &name) const;
		const XmlElement *getChild(const char *name) const;
		void addChild(XmlElement *child);

		std::string toString(int tabLevel = 0) const;
};

} // namespace DromeXml

#endif /* __DROMEXML_XMLELEMENT_H__ */
