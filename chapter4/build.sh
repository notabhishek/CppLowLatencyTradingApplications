# Create a build directory (recommended)
mkdir cmake-build-release && cd cmake-build-release

# Generate the build system
cmake .. -DCMAKE_CXX_COMPILER=clang++  # Or ensure clang++ is in your PATH

# Compile the project
make