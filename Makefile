CXX = g++
CXXFLAGS = -O3 -I. -IBigInt -IAlgebraicNumber -ICubicNumber -ICubicSolver \
           -IResultExporter -IUtilities -I./Utilities/libntl/include

# Prepojenie na NTL a GMP (vyžadované knižnicou NTL)
LDFLAGS = -L./Utilities/libntl/lib -lntl -lgmp

SOURCES = main.cpp \
          BigInt/BigInt.cpp \
          AlgebraicNumber/AlgebraicNumber.cpp \
          ResultExporter/ResultExporter.cpp \
          CubicNumber/CubicNumber.cpp \
          CubicSolver/CubicSolver.cpp \
          CubicSolver/CubicSolverLLL.cpp \
          Utilities/Math_bp.cpp

TARGET = cubic_solver

all:
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)