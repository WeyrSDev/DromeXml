/*
 * Copyright (C) 2011 Josh A. Beam
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

#include <iostream>
#include <DromeXml/DromeXml>

using namespace std;
using namespace DromeXml;

static void
printElement(const XmlElement *element, int tabs = 0)
{
	// print element name
	for(int i = 0; i < tabs; ++i)
		cout << '\t';
	cout << element->getName();

	// print attributes
	if(element->getNumAttributes() != 0) {
		cout << " (attributes: ";
		for(unsigned int i = 0; i < element->getNumAttributes(); ++i) {
			if(i != 0)
				cout << ", ";
			cout << element->getAttribute(i)->toString();
		}
		cout << ")";
	}

	cout << endl;

	// print children
	for(unsigned int i = 0; i < element->getNumChildren(); ++i)
		printElement(element->getChild(i), tabs + 1);
}

int
main(int /*argc*/, char * /*argv*/ [])
{
	const char *xml = "<?xml version=\"1.0\"?><root><child1 attr1=\"yes\" attr2=\"no\"><grandchild attr1=\"no\" /></child1><child2 attr1=\"5\" /></root>";

	XmlDocument doc;
	try {
		doc.fromString(xml);
		cout << "Parsed XML: " << endl << doc.toString() << endl;
		cout << endl << "Elements:" << endl;
		printElement(doc.getRootElement());
	} catch(XmlException ex) {
		cout << "EXCEPTION: " << ex.toString() << endl;
		return 1;
	}

	return 0;
}
