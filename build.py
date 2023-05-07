import subprocess

runCommand = '''mkdir build
cd build
rm -rf CMakeCache.txt
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j 15 
-DCMAKE_EXPORT_COMPILE_COMMANDS=1'''

subprocess.call([runCommand], shell=True)