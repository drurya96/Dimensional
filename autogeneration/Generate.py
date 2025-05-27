import json
from pathlib import Path
from metadata.python_utils.Dimension import AllDimension, FundamentalDimension
from jinja2 import Environment

env = Environment(
    trim_blocks=True,
    lstrip_blocks=True
)

def generate(obj, template_filepath, name_generator):

    with open(template_filepath, "r") as file:
        template_content = file.read()
    template = env.from_string(template_content)

    for item in obj:
        output = template.render(dim=item)
        output_filename = name_generator(item.name)
        with open(output_filename, "w") as f:
            f.write(output)
        print(f"Generated {output_filename}")

def main():
    # Load two JSON files
    file1 = Path("metadata/FundamentalUnits.json")
    file2 = Path("metadata/DerivedUnits.json")

    with file1.open() as f:
        fundamental_data = json.load(f)

    with file2.open() as f:
        derived_data = json.load(f)

    # Combine the two JSON lists
    combined_data = fundamental_data + derived_data
    
    # Parse into AllDimension
    all_dimensions = AllDimension.parse(combined_data)


    generate(
        all_dimensions.fundamental_dims.values(),
        "autogeneration/templates/FundamentalDimension.template",
        lambda name: f"Dimension/Dimension_Impl/FundamentalDimensions/{name}Dimension_Impl.h"
    )


    generate(
        all_dimensions.derived_dims.values(),
        "autogeneration/templates/DerivedDimension.template",
        lambda name: f"Dimension/Dimension_Impl/DerivedDimensions/{name}Dimension_Impl.h"
    )

    generate(
        all_dimensions.derived_dims.values(),
        "autogeneration/templates/DerivedUnit.template",
        lambda name: f"Dimension/{name}Dimension.h"
    )



if __name__ == "__main__":
    main()
