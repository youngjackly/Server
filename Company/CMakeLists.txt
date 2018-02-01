CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(company_sources
	client.cpp
	cliententry.cpp
	clientlist.cpp
	console.cpp
	eql_config.cpp
	login_server.cpp
	login_server_list.cpp
	net.cpp
	web_interface.cpp
	web_interface_eqw.cpp
	world_config.cpp
	world_console_connection.cpp
	worlddb.cpp
	zonelist.cpp
	zoneserver.cpp
)

SET(company_headers
	client.h
	cliententry.h
	clientlist.h
	console.h
	eql_config.h
	login_server.h
	login_server_list.h
	net.h
	web_interface.h
	web_interface_eqw.h
	world_config.h
	world_console_connection.h
	world_tcp_connection.h
	worlddb.h
	zonelist.h
	zoneserver.h
)

ADD_EXECUTABLE(company ${company_sources} ${company_headers})

INSTALL(TARGETS company RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)

ADD_DEFINITIONS(-DWORLD)

TARGET_LINK_LIBRARIES(company ${SERVER_LIBS})

IF(EQEMU_BUILD_PERL)
    TARGET_LINK_LIBRARIES(company ${PERL_LIBRARY})
ENDIF(EQEMU_BUILD_PERL)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
