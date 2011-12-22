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

#ifndef __DROMEXML_XMLTOKENIZER_H__
#define __DROMEXML_XMLTOKENIZER_H__

#include <string>

namespace DromeXml {

class XmlTokenizer
{
	private:
		enum State
		{
			STATE_SEARCHING_FOR_TOKEN = 0,
			STATE_TAG_START_FOUND,
			STATE_COMMENT0,
			STATE_COMMENT1,
			STATE_COMMENT2,
			STATE_TOKENIZING_TAG,
			STATE_TOKENIZING_IDENTIFIER,
			STATE_TOKENIZING_QUOTED_IDENTIFIER,
			STATE_TOKENIZING_CONTENT
		};

		State m_state;
		std::string m_data;
		char m_quoteChar;

	protected:
		unsigned int m_line;

		XmlTokenizer();
		void processCharacter(char c);
		void processData(const char *data);

		virtual void tokenFound(const char *token, bool quoted = false) = 0;
};

} // namespace DromeXml

#endif /* __DROMEXML_XMLTOKENIZER_H__ */
