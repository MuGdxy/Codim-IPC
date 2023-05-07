import subprocess

runCommand = '''
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ..
'''

subprocess.call([runCommand], shell=True)