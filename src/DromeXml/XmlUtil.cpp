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
#include "XmlUtil.h"

using namespace std;

namespace DromeXml {

static const char *ENTITIES[][2] = {
	{ "&nbsp;", " " },
	{ "&amp;", "&" },
	{ "&apos;", "'" },
	{ "&lt;", "<" },
	{ "&gt;", ">" },
	{ "&quot;", "\"" }
};

static const int NUM_ENTITIES = sizeof(ENTITIES) / sizeof(ENTITIES[0]);

bool
isWhitespace(const char c)
{
	return (c == ' ' || c == '\n' || c == '\r' || c == '\t');
}

string
decodeString(const string &s)
{
	size_t start;
	size_t pos = 0;
	string str = s;

	// look for xml entities in the string
	while((start = str.find('&', pos)) != string::npos) {
		size_t end = start + 1;

		pos = end;

		// find end of entity
		while((str[end] >= 'a' && str[end] <= 'z') ||
		      (str[end] >= 'A' && str[end] <= 'Z') ||
		      (str[end] >= '0' && str[end] <= '9') ||
		      str[end] == '#' || str[end] == 'x')
			++end;

		// last character must be a semicolon
		if(str[end] != ';')
			continue;

		size_t len = end - start + 1;

		// loop through known entities
		for(int i = 0; i < NUM_ENTITIES; ++i) {
			const char **entity = ENTITIES[i];

			// compare entity in string to known entity
			if(str.compare(start, len, entity[0]) == 0) {
				// replace entity in string with its corresponding character
				str.replace(start, len, entity[1]);
				break;
			}
		}
	}

	return str;
}

string
encodeString(const string &s)
{
	string str = s;

	// loop through each character in the string
	for(size_t i = 0; i < str.length(); ++i) {
		// loop through each known entity except for nbsp (index 0 in ENTITIES)
		for(int j = 1; j < NUM_ENTITIES; ++j) {
			const char **entity = ENTITIES[j];

			// compare characters
			if(str[i] == entity[1][0]) {
				// replace character with its corresponding entity
				str.replace(i, 1, entity[0]);
				i += strlen(entity[0]) - 1;
			}
		}
	}

	return str;
}

string
trimString(const string &s)
{
	size_t len;
	string str = s;

	// remove whitespace from the beginning
	while((len = str.length()) != 0 && isWhitespace(str[0]))
		str.erase(0, 1);

	// remove whitespace from the end
	while((len = str.length()) != 0 && isWhitespace(str[len - 1]))
		str.erase(len - 1, 1);

	return str;
}

} // namespace DromeXml
