#!/usr/bin/env python3
import subprocess

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
    print("DEFINITIONS")
    commands = [
        ["./extract.py", "macro", "SIMPLE_MACRO", "file_sample.h"],
        ["./extract.py", "macro", "LOGCRAZY", "file_sample.h"],
        ["./extract.py", "extern-function", "vAssertCalled", "file_sample.h"],
        ["./extract.py", "extern-variable", "MyExternVar", "file_sample.h"],
        ["./extract.py", "global", "LogTextOrigin", "file_sample.h", "-DLOG_ORIGIN_COUNT="],
        ["./extract.py", "function", "myFunction", "file_sample.h"],
        ["./extract.py", "global", "uwTick", "file_sample.h", "-D__IO="],
        ["./extract.py", "function", "HAL_IncTick", "file_sample.h", "-D__weak="],
        ["./extract.py", "prototype", "myFunction", "file_sample.h"],
        ["./extract.py", "typedef", "myTypedef", "file_sample.h"],
        ["./extract.py", "struct", "MyStruct", "file_sample.h"],
        ["./extract.py", "enum", "MyEnum", "file_sample.h"],
        ["./extract.py", "union", "MyUnion", "file_sample.h"],
        ["./extract.py", "global", "debugClient", "file_sample.h"],
        ["./extract.py", "prototype", "xTaskCreate", "file_sample.h", "-DconfigSUPPORT_DYNAMIC_ALLOCATION=1", "-DPRIVILEGED_FUNCTION="],
        ["./extract.py", "global", "deviceID", "file_sample.h"],
        ["./extract.py", "function", "SetupHardware", "file_sample.h"]
    ]

    for cmd in commands:
        run_command(cmd)

    print("\nLINES")
    commands_lines = [
        ["./extract.py", "macro", "SIMPLE_MACRO", "file_sample.h", "lines"],
        ["./extract.py", "macro", "LOGCRAZY", "file_sample.h", "lines"],
        ["./extract.py", "extern-function", "vAssertCalled", "file_sample.h", "lines"],
        ["./extract.py", "extern-variable", "MyExternVar", "file_sample.h", "lines"],
        ["./extract.py", "global", "LogTextOrigin", "file_sample.h", "lines", "-DLOG_ORIGIN_COUNT="],
        ["./extract.py", "function", "myFunction", "file_sample.h", "lines"],
        ["./extract.py", "global", "uwTick", "file_sample.h", "lines", "-D__IO="],
        ["./extract.py", "function", "HAL_IncTick", "file_sample.h", "lines", "-D__weak="],
        ["./extract.py", "prototype", "myFunction", "file_sample.h", "lines"],
        ["./extract.py", "typedef", "myTypedef", "file_sample.h", "lines"],
        ["./extract.py", "struct", "MyStruct", "file_sample.h", "lines"],
        ["./extract.py", "enum", "MyEnum", "file_sample.h", "lines"],
        ["./extract.py", "union", "MyUnion", "file_sample.h", "lines"],
        ["./extract.py", "global", "debugClient", "file_sample.h", "lines"],
        ["./extract.py", "prototype", "xTaskCreate", "file_sample.h", "lines", "-DconfigSUPPORT_DYNAMIC_ALLOCATION=1", "-DPRIVILEGED_FUNCTION="],
        ["./extract.py", "global", "deviceID", "file_sample.h", "lines"],
        ["./extract.py", "function", "SetupHardware", "file_sample.h", "lines"]
    ]

    for cmd in commands_lines:
        run_command(cmd)

if __name__ == "__main__":
    main()
