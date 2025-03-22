import os
import subprocess
import sys
import platform

def run_command(cmd):
    """Executa um comando e exibe sua saída."""
    print(f"\nExecutando: {' '.join(cmd)}")
    try:
        result = subprocess.run(cmd, capture_output=True, text=True, check=False)
        if result.stdout:
            print("Saída:")
            print(result.stdout)
        if result.stderr:
            print("Erros:")
            print(result.stderr)
    except Exception as e:
        print(f"Erro ao executar o comando: {e}")

def main():
    system = platform.system()
    # Se for Windows, use "python" ou "py" antes do script
    if system == "Windows":
        python_cmd = ["python"] 
    else:
        python_cmd = ["python3"]
        
    print("DEFINITIONS")
    commands = [
        python_cmd + ["extract.py", "macro", "SIMPLE_MACRO", "file_sample.h"],
        python_cmd + ["extract.py", "macro", "LOGCRAZY", "file_sample.h"],
        python_cmd + ["extract.py", "extern-function", "vAssertCalled", "file_sample.h"],
        python_cmd + ["extract.py", "extern-variable", "MyExternVar", "file_sample.h"],
        python_cmd + ["extract.py", "global", "LogTextOrigin", "file_sample.h", "-DLOG_ORIGIN_COUNT="],
        python_cmd + ["extract.py", "function", "myFunction", "file_sample.h"],
        python_cmd + ["extract.py", "global", "uwTick", "file_sample.h", "-D__IO="],
        python_cmd + ["extract.py", "function", "HAL_IncTick", "file_sample.h", "-D__weak="],
        python_cmd + ["extract.py", "prototype", "myFunction", "file_sample.h"],
        python_cmd + ["extract.py", "typedef", "myTypedef", "file_sample.h"],
        python_cmd + ["extract.py", "struct", "MyStruct", "file_sample.h"],
        python_cmd + ["extract.py", "enum", "MyEnum", "file_sample.h"],
        python_cmd + ["extract.py", "union", "MyUnion", "file_sample.h"],
        python_cmd + ["extract.py", "global", "debugClient", "file_sample.h"],
        python_cmd + ["extract.py", "prototype", "xTaskCreate", "file_sample.h", "-DconfigSUPPORT_DYNAMIC_ALLOCATION=1", "-DPRIVILEGED_FUNCTION="],
        python_cmd + ["extract.py", "global", "deviceID", "file_sample.h"],
        python_cmd + ["extract.py", "function", "SetupHardware", "file_sample.h"]
    ]

    for cmd in commands:
        run_command(cmd)

    print("\nLINES")
    commands_lines = [
        python_cmd + ["extract.py", "macro", "SIMPLE_MACRO", "file_sample.h", "lines"],
        python_cmd + ["extract.py", "macro", "LOGCRAZY", "file_sample.h", "lines"],
        python_cmd + ["extract.py", "extern-function", "vAssertCalled", "file_sample.h", "lines"],
        python_cmd + ["extract.py", "extern-variable", "MyExternVar", "file_sample.h", "lines"],
        python_cmd + ["extract.py", "global", "LogTextOrigin", "file_sample.h", "lines", "-DLOG_ORIGIN_COUNT="],
        python_cmd + ["extract.py", "function", "myFunction", "file_sample.h", "lines"],
        python_cmd + ["extract.py", "global", "uwTick", "file_sample.h", "lines", "-D__IO="],
        python_cmd + ["extract.py", "function", "HAL_IncTick", "file_sample.h", "lines", "-D__weak="],
        python_cmd + ["extract.py", "prototype", "myFunction", "file_sample.h", "lines"],
        python_cmd + ["extract.py", "typedef", "myTypedef", "file_sample.h", "lines"],
        python_cmd + ["extract.py", "struct", "MyStruct", "file_sample.h", "lines"],
        python_cmd + ["extract.py", "enum", "MyEnum", "file_sample.h", "lines"],
        python_cmd + ["extract.py", "union", "MyUnion", "file_sample.h", "lines"],
        python_cmd + ["extract.py", "global", "debugClient", "file_sample.h", "lines"],
        python_cmd + ["extract.py", "prototype", "xTaskCreate", "file_sample.h", "lines", "-DconfigSUPPORT_DYNAMIC_ALLOCATION=1", "-DPRIVILEGED_FUNCTION="],
        python_cmd + ["extract.py", "global", "deviceID", "file_sample.h", "lines"],
        python_cmd + ["extract.py", "function", "SetupHardware", "file_sample.h", "lines"]
    ]

    for cmd in commands_lines:
        run_command(cmd)

if __name__ == "__main__":
    main()
