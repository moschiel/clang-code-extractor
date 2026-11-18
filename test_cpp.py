"""Executable regression tests; uses the real Clang extractor."""
import os
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest
from concurrent.futures import ThreadPoolExecutor

ROOT = Path(__file__).resolve().parent


class CppExtractionTests(unittest.TestCase):
    def test_concurrent_calls_do_not_write_into_caller(self):
        with tempfile.TemporaryDirectory() as directory:
            directory = Path(directory)
            source = directory / 'input.c'
            source.write_text('int sample(void) { return 5; }\n')
            def extract(index):
                return subprocess.check_output([sys.executable, str(ROOT / 'extract.py'),
                    'function', 'sample', 'input.c', 'lines'], cwd=directory, text=True).strip()
            with ThreadPoolExecutor(max_workers=4) as pool:
                self.assertEqual(list(pool.map(extract, range(8))), ['1;1'] * 8)
            self.assertEqual({p.name for p in directory.iterdir()}, {'input.c'})

    def test_cpp_methods_namespaces_and_c_compatibility(self):
        cases = [
            ('unit.cpp', 'namespace device {\nclass Serial { public: int read(); };\nint Serial::read() { return 7; }\n}\n',
             'device::Serial::read', 'int Serial::read() { return 7; }', '3;3'),
            ('unit.cpp', 'namespace a { int step() { return 1; } }\nnamespace b { int step() { return 2; } }\n',
             'b::step', 'int step() { return 2; }', '2;2'),
            ('unit.c', 'int step(void) { return 3; }\n', 'step', 'int step(void) { return 3; }', '1;1'),
        ]
        with tempfile.TemporaryDirectory() as directory:
            for filename, code, symbol, expected, lines in cases:
                with self.subTest(symbol=symbol):
                    source = Path(directory) / filename
                    source.write_text(code)
                    cmd = [sys.executable, str(ROOT / 'extract.py'), 'function', symbol, str(source)]
                    result = subprocess.run(cmd, cwd=ROOT, capture_output=True, text=True)
                    self.assertEqual(result.returncode, 0, result.stderr + result.stdout)
                    self.assertEqual(result.stdout.strip(), expected)
                    result = subprocess.run(cmd + ['lines'], cwd=ROOT, capture_output=True, text=True)
                    self.assertEqual(result.returncode, 0, result.stderr + result.stdout)
                    self.assertEqual(result.stdout.strip(), lines)


if __name__ == '__main__':
    unittest.main()
