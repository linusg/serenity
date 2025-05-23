include(${SerenityOS_SOURCE_DIR}/Meta/CMake/public_suffix.cmake)

set(SOURCES
    Attribute.cpp
    ChromeProcess.cpp
    CookieJar.cpp
    Database.cpp
    InspectorClient.cpp
    ProcessHandle.cpp
    ProcessManager.cpp
    RequestServerAdapter.cpp
    SearchEngine.cpp
    SourceHighlighter.cpp
    URL.cpp
    UserAgent.cpp
    ViewImplementation.cpp
    WebContentClient.cpp
    WebSocketClientAdapter.cpp
    ${PUBLIC_SUFFIX_SOURCES}
)

set(GENERATED_SOURCES ${CURRENT_LIB_GENERATED})

if (SERENITYOS)
    list(APPEND SOURCES OutOfProcessWebView.cpp)
endif()

embed_as_string(
    "NativeStyleSheetSource.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Native.css"
    "NativeStyleSheetSource.cpp"
    "native_stylesheet_source"
    NAMESPACE "WebView"
)

compile_ipc(UIProcessServer.ipc UIProcessServerEndpoint.h)
compile_ipc(UIProcessClient.ipc UIProcessClientEndpoint.h)

set(GENERATED_SOURCES
    ${GENERATED_SOURCES}
    ../../Services/RequestServer/RequestClientEndpoint.h
    ../../Services/RequestServer/RequestServerEndpoint.h
    ../../Services/WebContent/WebContentClientEndpoint.h
    ../../Services/WebContent/WebContentServerEndpoint.h
    ../../Services/WebContent/WebDriverClientEndpoint.h
    ../../Services/WebContent/WebDriverServerEndpoint.h
    NativeStyleSheetSource.cpp
    UIProcessClientEndpoint.h
    UIProcessServerEndpoint.h
)

serenity_lib(LibWebView webview)
target_link_libraries(LibWebView PRIVATE LibCore LibFileSystem LibGfx LibIPC LibProtocol LibJS LibWeb LibSQL LibUnicode LibURL LibSyntax)
target_compile_definitions(LibWebView PRIVATE ENABLE_PUBLIC_SUFFIX=$<BOOL:${ENABLE_PUBLIC_SUFFIX_DOWNLOAD}>)

if (SERENITYOS)
    target_link_libraries(LibWebView PRIVATE LibFileSystemAccessClient LibGUI)
endif()

if (NOT SERENITYOS)
    foreach(header ${GENERATED_SOURCES})
        get_filename_component(extension ${header} EXT)
        if (NOT "${extension}" STREQUAL ".h")
            continue()
        endif()

        get_filename_component(subdirectory ${header} DIRECTORY)
        string(REGEX REPLACE "^\\.\\./\\.\\./" "" subdirectory "${subdirectory}")

        install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${header}" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${subdirectory}")
    endforeach()

    install(FILES "${SERENITY_PROJECT_ROOT}/Userland/Services/RequestServer/ConnectionCache.h" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/RequestServer")
endif()
