cmake_minimum_required(VERSION 3.12)
project(GroupVRProject)

# Find the VTK library on your computer
find_package(VTK REQUIRED COMPONENTS
    RenderingOpenGL2
    RenderingOpenVR
    InteractionStyle
    RenderingCore
    GUISupportQt
    FiltersSources
    IOGeometry
)

# Add your C++ file (make sure the name matches your .cpp file!)
add_executable(VRApp main.cpp ModelPart.h)

# Link VTK to your application
target_link_libraries(VRApp PRIVATE ${VTK_LIBRARIES})

# This part helps VTK initialize properly
vtk_module_autoinit(
  TARGETS VRApp
  MODULES VTK::RenderingOpenGL2
          VTK::RenderingOpenVR
          VTK::InteractionStyle
)

# Copy all VTK DLLs to the output folder after every build
add_custom_command(TARGET VRApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${VTK_DIR}/../../../bin/Release"
    $<TARGET_FILE_DIR:VRApp>
)