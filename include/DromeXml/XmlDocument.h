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

#ifndef __DROMEXML_XMLDOCUMENT_H__
#define __DROMEXML_XMLDOCUMENT_H__

#include "XmlElement.h"
#include "XmlParser.h"

namespace DromeXml {

class XmlDocument : public XmlParser
{
	protected:
		XmlElement *m_rootElement;
		XmlElement *m_currentElement;

		void parsedProcessingInstruction(const std::string &target);
		void parsedStartTag(const std::string &tagName);
		void parsedEndTag(const std::string &tagName);
		void parsedAttribute(const std::string &name, const std::string &value);
		void parsedContent(const std::string &content);

	public:
		XmlDocument();
		virtual ~XmlDocument();

		void fromFile(const char *filePath);
		void fromFile(const std::string &filePath);
		void fromString(const char *s);
		void fromString(const std::string &s);

		const XmlElement *getRootElement() const;
		void setRootElement(XmlElement *value);

		std::string toString() const;
};

} // namespace DromeXml

#endif /* __DROMEXML_XMLDOCUMENT_H__ */
