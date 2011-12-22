file(GLOB DromeXml "${CMAKE_CURRENT_SOURCE_DIR}/DromeXml/*")

install(
	FILES ${DromeXml}
	DESTINATION include/DromeXml
)
