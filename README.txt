# CS350
#Name - Junwoo Seo
#Digipen ID - junwoo.seo 
#Student Num - 0055213

#How to use parts of your user interface that is NOT specified in the assignment description.
It will be pretty straightforward to use it. GUI will help you to look around.
WSAD for move. 
Spacebar up
Ctrl down
Alt and mouse left click for rotate camera 

#Any assumption that you make on how to use the application that, if violated, might cause
the application to fail.
- For now, we need hardware that supports OpenGL version 4.6 and Windows Only. (using vs2022)

#Which part of the assignment has been completed? 
- Completed AABB, Ritter’s Method,Larsson’s Method, PCA-based Method, AABB BVH top down, AABB bottom up.
- Completed Control and UI 
- Completed ALL BVH.

#Which part of the assignment has NOT been completed (not done, not working, etc.) and
explanation on why those parts are not completed?
- no

#Where the relevant source codes (both C++ and shaders) for the assignment are located.
#Specify the file path (folder name), file name, and function name (or line number).

Core/Utils/Math.h and .cpp for BV compute.
Core/Layer/PhysicsLayer.h and cpp for BVH compute.

in Assets/Shaders folder there are shader sources for vertex and fragment shaders to draw Phong shaders and blinn shaders.
in RenderingEngine/Core/Graphics Folder for graphics stuff like FrameBuffers.
in RenderingEngine/Core/Data Folder for graphics data types.
For Shader files (Assets/Shaders/*.*)
For Scene setups Scene/*.cpp and Scene/*.h
For Deferred rendering FrameBuffer look Core/Layer/RenderLayer.cpp and look DeferredRender, DeferredRenderQuad and ForwardRender funtions.
For Deferred rendering Shader files look 4 shader files start with "deferred"



#Which machine did you test your application on. 
The application was tested on my machine.
OS: Windows11 Pro
GPU: NVIDIA GeForce GTX 1050/PCIe/
OpenGL Version: 4.6 NVIDIA 516.93

#The average number of hours you spent on the assignment, on a weekly basis
I spent 10 hours per week.

#Any other useful information pertaining to the application 
The default visual studio solution is for vs2022. however, you can make solutions for vs2019 and vs2022. 
I made a script that generates it with premake5.
