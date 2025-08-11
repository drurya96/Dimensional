#!/usr/bin/env python3
"""
merge_headers.py  –  Concatenate C/C++ headers for ChatGPT review.

Typical use:
    python merge_headers.py ./include  ./extras -o merged.hpp --strip --no-blank

Key features
------------
* Accepts any number of directories *or* individual files.
* Recurses into sub-folders with Path.rglob().
* Filters by extension (default: .h, .hpp, .hh, .hxx, .h++).
* Optional --strip to delete C/C++ comments.
* Optional --no-blank to remove all blank lines.
* Deterministic output order: sorted lexicographically by relative path.
"""
import argparse, pathlib, re, sys
from typing import Iterable, List
import tiktoken

DEFAULT_EXTS = {".h", ".hpp", ".hh", ".hxx", ".h++"}

_COMMENT_RX = re.compile(
    r"""//[^\n]*$           # // line comments
        | /\*.*?\*/         # /* block comments */
    """,
    re.MULTILINE | re.DOTALL | re.VERBOSE,
)


def strip_comments(text: str, remove_blank_lines: bool = False) -> str:
    text = _COMMENT_RX.sub("", text)
    if remove_blank_lines:
        text = "\n".join(line for line in text.splitlines() if line.strip())
    else:
        text = re.sub(r"\n{3,}", "\n\n", text)  # collapse ≥3 blank lines
    return text


def collect_inputs(paths: Iterable[str], exts: set[str], recursive: bool) -> List[pathlib.Path]:
    hdrs: List[pathlib.Path] = []
    for p in map(pathlib.Path, paths):
        if p.is_dir():
            globber = p.rglob if recursive else p.glob
            hdrs.extend([f for f in globber("*") if f.suffix in exts])
        elif p.suffix in exts:
            hdrs.append(p)
    # Use sorted relative paths for stable order
    hdrs.sort(key=lambda f: f.as_posix())
    return hdrs


def merge(files: List[pathlib.Path], strip: bool, no_blank: bool) -> str:
    parts = []
    for f in files:
        content = f.read_text(encoding="utf-8", errors="replace")
        if strip:
            content = strip_comments(content, remove_blank_lines=no_blank)
        parts.append(f"// ===== {f} =====\n{content}\n")
    return "\n".join(parts)


def tokenize(text: str) -> None:
    encoding = tiktoken.get_encoding("cl100k_base")
    tokens = encoding.encode(text)
    print(f"\nToken count: {len(tokens)}")


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument("inputs", nargs="+", help="header files or directories")
    ap.add_argument("-o", "--output", help="write result to this file (stdout if omitted)")
    ap.add_argument("--strip", action="store_true", help="remove comments")
    ap.add_argument("--no-blank", action="store_true", help="remove all blank lines (implies --strip)")
    ap.add_argument(
        "--exts",
        help=f"comma-separated list of header extensions (default: {','.join(sorted(DEFAULT_EXTS))})",
    )
    ap.add_argument("--no-recursive", action="store_true", help="disable directory recursion")
    args = ap.parse_args()

    exts = set(e if e.startswith(".") else f".{e}" for e in (args.exts.split(",") if args.exts else DEFAULT_EXTS))

    headers = collect_inputs(args.inputs, exts, recursive=not args.no_recursive)
    if not headers:
        sys.exit("No header files found – check paths/extensions.")

    blob = merge(headers, strip=(args.strip or args.no_blank), no_blank=args.no_blank)
    if args.output:
        pathlib.Path(args.output).write_text(blob, encoding="utf-8")
    else:
        sys.stdout.write(blob)

    tokenize(blob)


if __name__ == "__main__":
    main()
