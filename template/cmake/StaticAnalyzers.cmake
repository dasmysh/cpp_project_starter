option(ENABLE_CPPCHECK "Enable static analysis with cppcheck" OFF)
option(ENABLE_CLANG_TIDY "Enable static analysis with clang-tidy" OFF)
cmake_dependent_option(ENABLE_VS_STATICANALYSIS "Enable Visual Studio static analyzer" OFF "MSVC" OFF)
if(ENABLE_CPPCHECK)
  find_program(CPPCHECK cppcheck)
  if(CPPCHECK)
    set(CMAKE_CXX_CPPCHECK ${CPPCHECK} --suppress=missingInclude --enable=all
                           --inconclusive -i ${CMAKE_SOURCE_DIR}/imgui/lib)
  else()
    message(SEND_ERROR "cppcheck requested but executable not found")
  endif()
endif()

if(ENABLE_CLANG_TIDY)
  find_program(CLANGTIDY clang-tidy)
  if(CLANGTIDY)
    set(CMAKE_CXX_CLANG_TIDY ${CLANGTIDY};-header-filter=.;)
  else()
    if (NOT MSVC)
      message(SEND_ERROR "clang-tidy requested but executable not found")
    endif()
  endif()
endif()


function(set_project_static_analyzer project_name)
  if (MSVC AND (ENABLE_CLANG_TIDY OR ENABLE_VS_STATICANALYSIS))
    set_target_properties(${project_name} PROPERTIES VS_GLOBAL_RunCodeAnalysis true)
  endif()

  if (MSVC AND ENABLE_CLANG_TIDY)
    set_target_properties(${project_name} PROPERTIES
      VS_GLOBAL_EnableClangTidyCodeAnalysis true
      VS_GLOBAL_ClangTidyChecks -header-filter=.)
  endif()

  if (MSVC AND ENABLE_VS_STATICANALYSIS)
    set_target_properties(${project_name} PROPERTIES VS_GLOBAL_EnableMicrosoftCodeAnalysis true)
  endif()
endfunction()
