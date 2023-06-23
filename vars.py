cpp_source_folder = './cpp'
cpp_source_name = 'VPGen.cpp'
cpp_source_path = f'{cpp_source_folder}/{cpp_source_name}'

lib_folder = './'
lib_name = 'VPGenLib.so'
lib_path = f'{lib_folder}/{lib_name}'

data_folder = './data'
input_filename = 'metadata.json'
input_path = f'{data_folder}/{input_filename}'
output_filename = 'obstacles.json'
output_path = f'{data_folder}/{output_filename}'

lib_build_cmd = f'g++ -shared -fPIC {cpp_source_path} -o {lib_path}'
lib_build_cmd_wsl = f'wsl {lib_build_cmd}'

runner_cmd_wsl = f'wsl python3 ./VPGen.py {input_path}'
