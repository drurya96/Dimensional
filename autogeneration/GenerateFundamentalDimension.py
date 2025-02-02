import os
import sys
from jinja2 import Template
from collections import Counter

def main():
    if len(sys.argv) not in (3, 4):
        print("Usage: GenerateDerivedDimension.py <DimensionName> <NumeratorList> <DenominatorList>")
        return

    dimension_name = sys.argv[1]
    primary_name = sys.argv[2]


    # Construct parameters for the template
    template_params = {
        "dimension_name": dimension_name,
        "primary_name": primary_name
    }

    # Load the template from an external file
    template_path = "FundamentalDimension.template"
    with open(template_path, "r") as file:
        template_content = file.read()

    template = Template(template_content)
    output = template.render(**template_params)

    # Write the output file
    output_filename = f"Dimension/Dimension_Impl/FundamentalDimensions/{dimension_name}Dimension_Impl.h"
    with open(output_filename, "w") as f:
        f.write(output)

    print(f"Generated {output_filename}")

if __name__ == "__main__":
    main()
