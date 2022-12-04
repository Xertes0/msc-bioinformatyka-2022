if(DOXYGEN_ENABLED)
    set(DOXYGEN_PROJECT_NAME "MSC Bioinformatyka 2022")
    set(DOXYGEN_USE_MDFILE_AS_MAINPAGE README.md)

    set(DOXYGEN_GENERATE_HTML YES)
    set(DOXYGEN_GENERATE_MAN YES)
    set(DOXYGEN_JAVADOC_AUTOBRIEF YES)

    set(DOXYGEN_GENERATE_TREEVIEW YES)
    set(DOXYGEN_DISABLE_INDEX NO)
    set(DOXYGEN_FULL_SIDEBAR NO)

    set(DOXYGEN_DOT_IMAGE_FORMAT svg)
    set(DOXYGEN_DOT_TRANSPARENT YES)

    set(DOXYGEN_HTML_EXTRA_STYLESHEET
        ${doxygen-awesome_SOURCE_DIR}/doxygen-awesome.css
        ${doxygen-awesome_SOURCE_DIR}/doxygen-awesome-sidebar-only.css
        ${doxygen-awesome_SOURCE_DIR}/doxygen-awesome-sidebar-only-darkmode-toggle.css
    )

    set(DOXYGEN_HTML_EXTRA_FILES
        ${doxygen-awesome_SOURCE_DIR}/doxygen-awesome-darkmode-toggle.js
        ${doxygen-awesome_SOURCE_DIR}/doxygen-awesome-interactive-toc.js
    )

    set(DOXYGEN_HTML_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/header.html)

    doxygen_add_docs(
        docs
        ${CMAKE_SOURCE_DIR}/README.md
        ${CMAKE_SOURCE_DIR}/biolib
    )
endif()
