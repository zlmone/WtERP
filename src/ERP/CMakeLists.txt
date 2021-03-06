#Link Directories
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${GM_INCLUDE_DIRS}/Magick++/lib)
INCLUDE_DIRECTORIES(${GM_INCLUDE_DIRS})

#Get Files Lists
FILE(GLOB_RECURSE sources_dbo Dbo/*.cpp Dbo/*.h)
FILE(GLOB_RECURSE sources_application Application/*.cpp Application/*.h)
FILE(GLOB_RECURSE sources_widgets Widgets/*.cpp Widgets/*.h)
FILE(GLOB_RECURSE sources_modelview ModelView/*.cpp ModelView/*.h)
FILE(GLOB sources_localdir *.cpp *.h)
#Group File
SOURCE_GROUP(Dbo FILES ${sources_dbo})
SOURCE_GROUP(Application FILES ${sources_application})
SOURCE_GROUP(Widgets FILES ${sources_widgets})
SOURCE_GROUP(ModelView FILES ${sources_modelview})
SOURCE_GROUP("" FILES ${sources_localdir})

#Concatenate Files Lists
SET(ERP_SRCS
	${sources_dbo}
	${sources_application}
	${sources_widgets}
	${sources_modelview}
	${sources_localdir}
)
#Set Target/Project name
SET(WT_PROJECT_TARGET ERP)

#Add Program CXX files
ADD_EXECUTABLE(${WT_PROJECT_TARGET} ${ERP_SRCS})

#Parallel compile
IF(MSVC)
	SET_TARGET_PROPERTIES(ERP PROPERTIES COMPILE_FLAGS "/MP3 /wd4297 /wd4251 /wd4275 /wd4267")
ENDIF(MSVC)

#Link Libraries
TARGET_LINK_LIBRARIES(${WT_PROJECT_TARGET}
	PUBLIC
	wthttp
	wt
	wtdbo
	wtdbosqlite3
	wtdbomysql
	${GM_LIBRARIES}
	PRIVATE ${CMAKE_THREAD_LIBS_INIT}
#	wtdbofirebird
#	wtdbopostgres
)
