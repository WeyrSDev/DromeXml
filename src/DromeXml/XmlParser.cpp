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

#include <cstring>
#include <DromeXml/XmlException.h>
#include <DromeXml/XmlParser.h>

using namespace std;

namespace DromeXml {

/*
 * XmlParser class
 */
XmlParser::XmlParser()
{
	m_state = STATE_SEARCHING_FOR_ELEMENT;
}

void
XmlParser::tokenFound(const char *token, bool quoted)
{
#ifdef XMLDEBUG
	cout << "STATE: " << m_state << endl;
	cout << "TOKEN: " << token << endl;
#endif /* XMLDEBUG */

	switch(m_state) {
		default:
		case STATE_SEARCHING_FOR_ELEMENT:
			// if this is a tag, start searching for the name;
			// otherwise, this is content
			if(strcmp(token, "<") == 0)
				m_state = STATE_PARSING_TAG;
			else
				parsedContent(string(token));

			break;

		case STATE_PARSING_TAG:
			// processing instruction
			if(strcmp(token, "?") == 0) {
				m_state = STATE_PARSING_TAG_NAME_PI;
				break;
			}

			// end tag
			if(strcmp(token, "/") == 0) {
				m_state = STATE_PARSING_END_TAG_NAME;
				break;
			}

			// start tag
			m_state = STATE_PARSING_ATTRIBUTES;
			parsedStartTag(string(token));
			break;

		case STATE_PARSING_TAG_NAME:
		case STATE_PARSING_TAG_NAME_PI:
		case STATE_PARSING_END_TAG_NAME:
			// name must be non-quoted
			if(quoted)
				throw XmlException(m_line, "XmlParser::tokenFound(): Expected non-quoted tag name");

			// name must not be a special single-character token
			if(strcmp(token, "<") == 0 || strcmp(token, "!") == 0 ||
			   strcmp(token, "?") == 0 || strcmp(token, "/") == 0 ||
			   strcmp(token, ">") == 0)
				throw XmlException(m_line, "XmlParser::tokenFound(): Expected tag name");

			if(m_state == STATE_PARSING_TAG_NAME) {
				m_state = STATE_PARSING_ATTRIBUTES;
				parsedStartTag(string(token));
			} else if(m_state == STATE_PARSING_TAG_NAME_PI) {
				m_state = STATE_PARSING_ATTRIBUTES_PI;
				parsedProcessingInstruction(string(token));
			} else {
				m_state = STATE_PARSING_TAG_END;
				parsedEndTag(string(token));
			}

			break;

		case STATE_PARSING_ATTRIBUTES:
			// if this is a slash, this is a tag with no content or children
			if(strcmp(token, "/") == 0) {
				m_state = STATE_PARSING_TAG_END;
				parsedEndTag(string(""));
				break;
			}

			// end of attributes; search for content/children
			if(strcmp(token, ">") == 0) {
				m_state = STATE_SEARCHING_FOR_ELEMENT;
				break;
			}

			m_state = STATE_PARSING_ATTRIBUTE_DELIMITER;
			m_data = token;
			break;

		case STATE_PARSING_ATTRIBUTES_PI:
			// if this is a question mark, there are no more attributes
			if(strcmp(token, "?") == 0) {
				m_state = STATE_PARSING_TAG_END;
				break;
			}

			m_state = STATE_PARSING_ATTRIBUTE_DELIMITER_PI;
			m_data = token;
			break;

		case STATE_PARSING_ATTRIBUTE_DELIMITER:
		case STATE_PARSING_ATTRIBUTE_DELIMITER_PI:
			// must be an equals sign
			if(strcmp(token, "=") == 0) {
				if(m_state == STATE_PARSING_ATTRIBUTE_DELIMITER)
					m_state = STATE_PARSING_ATTRIBUTE_VALUE;
				else
					m_state = STATE_PARSING_ATTRIBUTE_VALUE_PI;
				break;
			}

			throw XmlException(m_line, (string("XmlParser::tokenFound(): Expected '=', got '") + token + "'").c_str());
			break;

		case STATE_PARSING_ATTRIBUTE_VALUE:
			m_state = STATE_PARSING_ATTRIBUTES;
			parsedAttribute(m_data, string(token));
			break;

		case STATE_PARSING_ATTRIBUTE_VALUE_PI:
			m_state = STATE_PARSING_ATTRIBUTES_PI;
			parsedAttribute(m_data, string(token));
			break;

		case STATE_PARSING_TAG_END:
			// this must be the end of the tag
			if(strcmp(token, ">") == 0) {
				m_state = STATE_SEARCHING_FOR_ELEMENT;
				break;
			}

			throw XmlException(m_line, (string("XmlParser::tokenFound(): Expected '>', got '") + token + "'").c_str());
			break;
	}
}

} // namespace DromeXml
