from tree_sitter import Language, Parser
from tree_sitter import Node, Tree
from typing import Dict, List
from dataclasses import dataclass
import subprocess
import tree_sitter_cpp as tscpp
import os

@dataclass
class compile_test:
    """
    Represents a single compile-time test case with associated metadata.
    """
    id: str
    expect_compile: bool
    description: str
    struct_name: str

    @classmethod
    def from_node(cls, node: Node) -> "compile_test":
        """
        Constructs a compile_test instance from a Tree-sitter AST node.
        
        Args:
            node: A Tree-sitter node representing a C++ struct.
        
        Returns:
            compile_test: The parsed metadata and struct name packaged into an instance.
        """
        metadata = cls._extract_metadata(node)
        cls._validate_metadata(metadata)
        struct_name = node.child_by_field_name("name").text.decode()
        return cls(
            id=metadata["id"],
            expect_compile=metadata["expect_error"].lower() != "true",
            description=metadata["description"],
            struct_name=struct_name,
        )

    @staticmethod
    def _extract_metadata(node: Node) -> Dict[str, str]:
        """
        Extracts metadata fields from the body of a struct.
        
        Args:
            node: A Tree-sitter node representing the struct.
        
        Returns:
            A dictionary containing the metadata keys and values.
        """
        metadata = {}
        body = node.child_by_field_name("body")
        if body is None:
            return metadata

        for field in body.named_children:
            if field.type != "field_declaration":
                continue

            declarator = field.child_by_field_name("declarator")
            value_node = field.child_by_field_name("default_value")
            if not declarator or not value_node:
                continue

            key = compile_test._parse_metadata_field(declarator.text.decode())
            value = value_node.text.decode().strip('"')
            metadata[key] = value
        return metadata

    @staticmethod
    def _parse_metadata_field(field_name: str) -> str:
        """
        Normalizes field names by removing pointer indicators or spacing.
        
        Args:
            field_name: The raw field name string from the AST.
        
        Returns:
            A cleaned-up metadata key.
        """
        return field_name.lstrip("* ")  # Remove pointer notation or spacing

    @staticmethod
    def _validate_metadata(metadata: Dict[str, str]) -> None:
        """
        Validates the presence of all required metadata fields.
        
        Args:
            metadata: Dictionary of extracted metadata.
        
        Raises:
            ValueError if any required field is missing.
        """
        required_keys = ["id", "expect_error", "description"]
        missing = [key for key in required_keys if key not in metadata]
        if missing:
            raise ValueError(f"Missing required metadata: {missing}")

def find_tests(tree: Tree) -> List[compile_test]:
    """
    Recursively searches for all compile test structs in the AST.
    
    Args:
        tree: The parsed Tree-sitter syntax tree.
    
    Returns:
        A list of compile_test instances.
    """
    def walk(node):
        found = []
        if node.type == "struct_specifier":
            try:
                found.append(compile_test.from_node(node))
            except Exception as e:
                print(f"Warning: {e}")
        for child in node.children:
            found.extend(walk(child))
        return found
    return walk(tree.root_node)

@dataclass
class compiler_settings:
    """
    Encapsulates settings for calling the C++ compiler.
    """
    compiler: str = "g++"
    flags: List[str] = None

    def __post_init__(self):
        """
        Provides default flags if none are specified.
        """
        if self.flags is None:
            self.flags = ["-std=c++20", "-c", "-x", "c++", "-"]

def compile_source(test: compile_test, source: str, settings: compiler_settings) -> bool:
    """
    Compiles a test struct by invoking the configured compiler.
    
    Args:
        struct_name: The name of the struct to test.
        source: The main source file to compile.
        settings: The compiler and flags to use.
    
    Returns:
        True if the compilation succeeded, False otherwise.
    """
    struct_name = test.struct_name
    result = subprocess.run(
        [
            settings.compiler,
            str(source),
            "/D", f"TEST_STRUCT_NAME={struct_name}",
            *settings.flags
        ],
        capture_output=True,
        env=settings.env
    )

    success = result.returncode == 0
    if success == test.expect_compile:
        print(f"[PASS] {test.id}: {test.description}")
    else:
        print(f"[FAIL] {test.id}: {test.description} (expected {'compile' if test.expect_compile else 'failure'})")
        if success:
            print("  [Unexpected Compile]")
        else:
            print("  [Unexpected Error Output]")
            print("  --- Compiler STDOUT ---")
            print(result.stdout.decode(errors="replace"))
            print("  --- Compiler STDERR ---")
            print(result.stderr.decode(errors="replace"))
            print("  ----------------------")

    return result.returncode == 0

def compile_all_tests(tree: Tree, settings: compiler_settings) -> None:
    """
    Finds all tests in the syntax tree and compiles each one.
    
    Args:
        tree: The parsed syntax tree.
        settings: Compiler configuration to use for each test.
    """
    tests = find_tests(tree)
    for test in tests:
        success = compile_source(test, "main.cpp", settings)
        """
        if success == test.expect_compile:
            print(f"[PASS] {test.id}: {test.description}")
        else:
            print(f"[FAIL] {test.id}: {test.description} (expected {'compile' if test.expect_compile else 'failure'})")
        """

def parse_header(filename: str) -> Tree:
    """
    Parses the given C++ header file into a Tree-sitter syntax tree.
    
    Args:
        filename: Path to the C++ header file.
    
    Returns:
        A Tree-sitter Tree object representing the parsed source.
    """
    CPP_LANGUAGE = Language(tscpp.language())
    parser = Parser(CPP_LANGUAGE)
    with open(filename, 'rb') as f:
        code = f.read()
    return parser.parse(code)


def get_msvc_env():
    vcvars_path = r"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
    command = f'"{vcvars_path}" && set'
    output = subprocess.check_output(command, shell=True, text=True)
    env = {}
    for line in output.splitlines():
        if "=" in line:
            key, value = line.split("=", 1)
            env[key] = value
    return env

def main():
    """
    Entry point. Parses the test header and compiles all detected tests.
    """
    tree = parse_header('dimensional_compile_test.hpp')

    include_path = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..", "..", "Dimension"))

    settings = compiler_settings(
        compiler=r"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.42.34433\bin\Hostx64\x64\cl.exe",
        flags=[
            "/std:c++20",                  # Enable C++20
            "/nologo",                     # Suppress startup banner
            "/c",                          # Compile only, no linking
            "/EHsc",                       # Enable standard C++ exception handling
            f"/I{include_path}"   # Include Dimensional headers
        ]
    )

    settings.env = get_msvc_env()

    compile_all_tests(tree, settings)

if __name__ == "__main__":
    main()
