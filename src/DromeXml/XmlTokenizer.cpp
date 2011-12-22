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
#include <DromeXml/XmlTokenizer.h>
#include "XmlUtil.h"

using namespace std;

namespace DromeXml {

/*
 * XmlTokenizer class
 */
XmlTokenizer::XmlTokenizer()
{
	m_state = STATE_SEARCHING_FOR_TOKEN;
	m_line = 1;
}

void
XmlTokenizer::processCharacter(char c)
{
	// increment line count if this
	// is a newline character
	if(c == '\n')
		++m_line;

	switch(m_state) {
		default:
		case STATE_SEARCHING_FOR_TOKEN:
			// skip whitespace characters
			if(isWhitespace(c))
				break;

			// start of tag found
			if(c == '<') {
				m_state = STATE_TAG_START_FOUND;
				break;
			}

			// start of content found
			m_state = STATE_TOKENIZING_CONTENT;
			m_data = c;
			break;

		case STATE_COMMENT0:
		case STATE_COMMENT1:
			// expecting "-"
			if(c == '-') {
				if(m_state == STATE_COMMENT0) {
					m_state = STATE_COMMENT1;
				} else {
					m_state = STATE_COMMENT2;
					// STATE_COMMENT2 requires m_data to be 3 characters long
					m_data = "   ";
				}
				break;
			}

			throw XmlException(m_line, "XmlTokenizer::processCharacter(): Invalid comment format - expected \"-\"");

		case STATE_COMMENT2:
			m_data = (m_data + c).substr(1);

			// end of comment
			if(m_data == "-->") {
				m_state = STATE_SEARCHING_FOR_TOKEN;
				m_data = "";
			}

			break;

		case STATE_TAG_START_FOUND:
			// start of comment
			if(c == '!') {
				m_state = STATE_COMMENT0;
				break;
			}

			// start of another tag type - emit the "<"
			// token and go to the STATE_TOKENIZING_TAG case
			m_state = STATE_TOKENIZING_TAG;
			tokenFound("<");

		case STATE_TOKENIZING_TAG:
			// skip whitespace characters
			if(isWhitespace(c))
				break;

			// single character tokens
			if(c == '/' || c == '!' || c == '?' || c == '=') {
				m_data = c;
				tokenFound(m_data.c_str());
				break;
			}

			// end of tag found
			if(c == '>') {
				m_state = STATE_SEARCHING_FOR_TOKEN;
				tokenFound(">");
				break;
			}

			// start of quoted identifier
			if(c == '"' || c == '\'') {
				m_state = STATE_TOKENIZING_QUOTED_IDENTIFIER;
				m_data.clear();
				m_quoteChar = c;
				break;
			}

			// start of non-quoted identifier
			m_state = STATE_TOKENIZING_IDENTIFIER;
			m_data = c;
			break;

		case STATE_TOKENIZING_QUOTED_IDENTIFIER:
			// end of quoted identifier
			if(c == m_quoteChar) {
				m_state = STATE_TOKENIZING_TAG;
				tokenFound(m_data.c_str(), true);
				break;
			}

			m_data += c;
			break;

		case STATE_TOKENIZING_IDENTIFIER:
			// whitespace ends identifier
			if(isWhitespace(c)) {
				m_state = STATE_TOKENIZING_TAG;
				tokenFound(m_data.c_str());
				break;
			}

			// single character tokens
			if(c == '/' || c == '!' || c == '?' || c == '=') {
				m_state = STATE_TOKENIZING_TAG;
				tokenFound(m_data.c_str());
				m_data = c;
				tokenFound(m_data.c_str());
				break;
			}

			// end of tag found
			if(c == '>') {
				m_state = STATE_SEARCHING_FOR_TOKEN;
				tokenFound(m_data.c_str());
				tokenFound(">");
				break;
			}

			m_data += c;
			break;

		case STATE_TOKENIZING_CONTENT:
			// start of tag found
			if(c == '<') {
				m_state = STATE_TOKENIZING_TAG;
				tokenFound(trimString(m_data).c_str());
				tokenFound("<");
				break;
			}

			m_data += c;
			break;
	}
}

void
XmlTokenizer::processData(const char *data)
{
	size_t len = strlen(data);
	for(size_t i = 0; i < len; ++i)
		processCharacter(data[i]);
}

} // namespace DromeXml
