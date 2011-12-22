set(
	SRCS
	XmlAttribute.cpp
	XmlDocument.cpp
	XmlElement.cpp
	XmlException.cpp
	XmlParser.cpp
	XmlTokenizer.cpp
	XmlUtil.cpp
)

add_library(DromeXml STATIC ${SRCS})

install(
	TARGETS DromeXml
	RUNTIME DESTINATION ${DROME_LIBRARY_DIR}
	LIBRARY DESTINATION ${DROME_LIBRARY_DIR}
	ARCHIVE DESTINATION ${DROME_LIBRARY_DIR}
)
