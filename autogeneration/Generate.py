import json
from pathlib import Path
from metadata.python_utils.unit_model import (
    DimensionalUnitsDimensions,
    FundamentalBlock,
    DerivedBlock,
)
from jinja2 import Environment
from itertools import permutations
from typing import Iterable
from .utils import build_fundamental_render_items, build_derived_render_items, build_derived_impl_items, build_header_context

def constraint_permutations(dim_def):
    """
    Accepts a sequence of items where each item is either:
      - a dict with key 'dim' (our normalized render dicts), or
      - a Pydantic object with attribute .dim or .Dimension.
    Returns a list of permutations of constraint strings like 'is_length_unit'.
    """
    names = []
    for d in dim_def:
        if isinstance(d, dict):
            name = d.get("dim") or d.get("Dimension")
        else:
            name = getattr(d, "dim", None) or getattr(d, "Dimension", None)
        if not name:
            raise ValueError(f"constraint_permutations: cannot extract dimension name from {d!r}")
        names.append(str(name))
    constraints = [f"is_{n}_unit" for n in names]
    return list(permutations(constraints))

env = Environment(
    trim_blocks=True,
    lstrip_blocks=True,
)
env.globals["constraint_permutations"] = constraint_permutations

def generate(items: Iterable, template_filepath: str, name_generator):
    with open(template_filepath, "r") as file:
        template_content = file.read()
    template = env.from_string(template_content)

    for item in items:
        output = template.render(dim=item)  # item can be a Pydantic model or a dict
        output_filename = name_generator(item)
        Path(output_filename).parent.mkdir(parents=True, exist_ok=True)
        with open(output_filename, "w") as f:
            f.write(output)
        print(f"Generated {output_filename}")

def generate_entry_header(all_blocks, template_filepath: str):
    with open(template_filepath, "r") as file:
        template_content = file.read()
    template = env.from_string(template_content)
    output = template.render(dims=all_blocks)
    output_filename = "Dimension/dimensions/dimensions.h"
    Path(output_filename).parent.mkdir(parents=True, exist_ok=True)
    with open(output_filename, "w") as f:
        f.write(output)

def main():
    file1 = Path("metadata/FundamentalUnits.json")
    file2 = Path("metadata/DerivedUnits.json")

    with file1.open() as f:
        fundamental_data = json.load(f)
    with file2.open() as f:
        derived_data = json.load(f)

    combined_data = fundamental_data + derived_data
    model = DimensionalUnitsDimensions.model_validate(combined_data)
    blocks = model.root

    fundamentals = [b for b in blocks if isinstance(b, FundamentalBlock)]
    derived = [b for b in blocks if isinstance(b, DerivedBlock)]

    # --- Fundamental Dimensions (impl) ---
    render_items_impl = build_fundamental_render_items(fundamentals)
    generate(
        render_items_impl,
        "autogeneration/templates/FundamentalDimension.template",
        lambda item: f"Dimension/Dimension_Impl/FundamentalDimensions/{item['Dimension']}_dimension_Impl.h",
    )

    # --- Fundamental Units (headers) ---  [ACTIVE]
    render_items = build_fundamental_render_items(fundamentals)
    generate(
        render_items,
        "autogeneration/templates/FundamentalUnit.template",
        lambda item: f"Dimension/dimensions/fundamental/{item['Dimension']}_dimension.h",
    )

    # --- Derived Units (headers) ---
    render_items_derived = build_derived_render_items(derived)
    generate(
        render_items_derived,
        "autogeneration/templates/DerivedUnit.template",
        lambda item: f"Dimension/dimensions/derived/{item['Dimension']}_dimension.h",
    )
    

    # --- Derived Units (headers) ---
    
    render_items_derived_impl = build_derived_impl_items(derived)
    generate(
        render_items_derived_impl,
        "autogeneration/templates/DerivedDimension.template",
        lambda item: f"Dimension/Dimension_Impl/DerivedDimensions/{item['Dimension']}_dimension_Impl.h",
    )
    

    # --- All-dimensions entry header ---
    ctx = build_header_context(fundamentals, derived)
    generate_entry_header(ctx, "autogeneration/templates/all_dimension_header.template")
    

if __name__ == "__main__":
    main()
