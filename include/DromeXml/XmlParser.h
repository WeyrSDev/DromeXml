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

#ifndef __DROMEXML_XMLPARSER_H__
#define __DROMEXML_XMLPARSER_H__

#include "XmlTokenizer.h"

namespace DromeXml {

class XmlParser : public XmlTokenizer
{
	protected:
		enum State
		{
			STATE_SEARCHING_FOR_ELEMENT = 0,
			STATE_PARSING_TAG,
			STATE_PARSING_TAG_NAME,
			STATE_PARSING_TAG_NAME_PI,
			STATE_PARSING_END_TAG_NAME,
			STATE_PARSING_ATTRIBUTES,
			STATE_PARSING_ATTRIBUTES_PI,
			STATE_PARSING_ATTRIBUTE_DELIMITER,
			STATE_PARSING_ATTRIBUTE_DELIMITER_PI,
			STATE_PARSING_ATTRIBUTE_VALUE,
			STATE_PARSING_ATTRIBUTE_VALUE_PI,
			STATE_PARSING_QUOTED_ATTRIBUTE_VALUE,
			STATE_PARSING_TAG_END,
			STATE_PARSING_CONTENT
		};

		State m_state;
		std::string m_data;

		XmlParser();

		void tokenFound(const char *token, bool quoted);

		virtual void parsedProcessingInstruction(const std::string &target) = 0;
		virtual void parsedStartTag(const std::string &tagName) = 0;
		virtual void parsedEndTag(const std::string &tagName) = 0;
		virtual void parsedAttribute(const std::string &name, const std::string &value) = 0;
		virtual void parsedContent(const std::string &content) = 0;
};

} // namespace DromeXml

#endif /* __DROMEXML_XMLPARSER_H__ */
