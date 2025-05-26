set(target ${MP2_APP})

option(BUILD_APP_MODULE "Include app_pamm in project? (YES/NO)" YES)

if (BUILD_APP_MODULE)

    # Find includes in corresponding build directories
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
    # Instruct CMake to run moc automatically when needed
    set(CMAKE_AUTOMOC ON)

    set(ZLIB_USE_STATIC_LIBS "ON")
    set(ZLIB_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../libs/zlib")
    find_package(ZLIB REQUIRED)

    # Указать путь к папке с DLL
    if (NOT DEFINED QT_PATH)
        set(QT_PATH "C:/Qt/6.8.2/msvc2022_64")
    endif()
    set(QT_DLL_PATH "${QT_PATH}/bin")  # Путь к DLL
    set(QT_PLUGIN_PATH "${QT_PATH}/plugins")  # Путь к плагинам
    set(Qt6_DIR "${QT_PATH}/lib/cmake/Qt6")
    find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui)


    file(GLOB_RECURSE ui "${CMAKE_CURRENT_SOURCE_DIR}/ui/*.ui")
    file(GLOB_RECURSE hdrs "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h*" "${MP2_INCLUDE}/*.h*")
    file(GLOB_RECURSE srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

    qt6_wrap_ui(GENERATED_UI_HEADERS ${ui})

    qt_add_executable(${target} ${hdrs} ${srcs} ${GENERATED_UI_HEADERS})
    qt_add_resources(RESOURCES resources.qrc)
    target_link_libraries(${target} PUBLIC ${MP2_LIBRARY} Qt6::Core Qt6::Gui Qt6::Widgets ${ZLIB_LIBRARIES})
    target_sources(${target} PRIVATE ${RESOURCES})

    add_dependencies(${target} ${MP2_COMPILER_APP})

    # Список нужных DLL
    set(QT_DLLS
        "Qt6Cored.dll"
        "Qt6Guid.dll"
        "Qt6Widgetsd.dll"
    )

    # Копирование DLL в папку с exe
    foreach(DLL ${QT_DLLS})
        add_custom_command(TARGET ${target} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${QT_DLL_PATH}/${DLL}" 
            "$<TARGET_FILE_DIR:${target}>"
        )
    endforeach()

    # Копирование всей папки с плагинами
    add_custom_command(TARGET ${target} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${QT_PLUGIN_PATH}/platforms"
        "$<TARGET_FILE_DIR:${target}>/platforms"
    )

endif()