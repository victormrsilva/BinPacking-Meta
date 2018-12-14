cmake_minimum_required(VERSION 3.5)
project(binpack)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_STANDARD 99)

set(LP_COMPILE_FLAGS "-DGRB -I/opt/gurobi800/linux64/include")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -Wall -g3  ${LP_COMPILE_FLAGS}")
set(LP_LINK_FLAGS "-L/opt/gurobi800/linux64/lib -lgurobi80 -lgurobi_c++")


add_executable(
  binpack main.cpp Container.cpp Container.h Instance.cpp Instance.h Solution.h Solution.cpp
  GRASP.h GRASP.cpp BuscaLocal.h BuscaLocal.cpp lp.h lp.cpp SA.h SA.cpp Decoder.cpp Decoder.h Solver.h Solver.cpp 
  brkgaAPI/Population.cpp brkgaAPI/Population.h brkgaAPI/MTRand.h  brkgaAPI/BRKGA.h 

)

target_link_libraries(binpack ${LP_LINK_FLAGS})