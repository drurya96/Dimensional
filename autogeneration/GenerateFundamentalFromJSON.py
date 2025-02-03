from jinja2 import Environment
import json

import Dimension

def validate(dim):
    return dim.name and dim.primary

def main():

    with open("../metadata/FundamentalUnits.json", 'r') as f:
        data = json.load(f)

    # Load the template from an external file
    template_path = "templates/FundamentalDimension.template"
    with open(template_path, "r") as file:
        template_content = file.read()

    env = Environment(
        trim_blocks=True,
        lstrip_blocks=True
    )

    template = env.from_string(template_content)

    for obj in data:
        dim = Dimension.dimension()
        dim.init_from_json(obj)

        if validate(dim):

            output = template.render(dim=dim)

            # Write the output file
            output_filename = f"../Dimension/Dimension_Impl/FundamentalDimensions/{dim.name}Dimension_Impl.h"
            with open(output_filename, "w") as f:
                f.write(output)

            print(f"Generated {output_filename}")

        else:
            print("Failed to generate")

if __name__ == "__main__":
    main()
