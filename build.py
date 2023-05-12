import subprocess

runCommand = '''mkdir build
cd build
rm -rf CMakeCache.txt
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ..
make -j 15 
'''

runCommand = '''mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ..
make -j 15 
'''

subprocess.call([runCommand], shell=True)