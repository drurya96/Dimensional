import sys
from jinja2 import Environment
from collections import Counter


class dimension_list:
    def __init__(self):
        self.list = []
        self.list_numbered = []
        self.unique_dims = []
        self.size = 0

    def make_numbered_list(self):
        output_list_counter = Counter()
        self.list_numbered = [
            f"{unit}{output_list_counter.update([unit]) or output_list_counter[unit]}"
            for unit in self.list
        ]
    
    def set_list(self, incoming_list):
        self.list = incoming_list
        self.unique_dims = set(self.list)
        self.make_numbered_list()
        self.size = len(self.list)


class dimension:
    def __init__(self):
        self.name = ""
        self.num = dimension_list()
        self.den = dimension_list()
        self.dimension_checks = ""

    def set_num_list(self, incoming_list):
        self.num.set_list(incoming_list)

    def set_den_list(self, incoming_list):
        self.den.set_list(incoming_list)


def main():
    if len(sys.argv) not in (3, 4):
        print("Usage: GenerateDerivedDimension.py <DimensionName> <NumeratorList> <DenominatorList>")
        return

    dimension_name = sys.argv[1]
    numerator_list = sys.argv[2].split(",")
    denominator_list = sys.argv[3].split(",")

    if numerator_list == ["[]"]:
        numerator_list = []

    if denominator_list == ["[]"]:
        denominator_list = []

    dim = dimension()
    dim.name = dimension_name
    dim.set_num_list(numerator_list)
    dim.set_den_list(denominator_list)

    # Re-evaluate this...
    dim.dimension_checks = " &&\n        ".join([f"std::is_same_v<typename {unit}::Dim, {unit[:-1]}Type>" for unit in dim.num.list_numbered + dim.den.list_numbered])

    # Load the template from an external file
    template_path = "templates/DerivedDimension.template"
    with open(template_path, "r") as file:
        template_content = file.read()

    env = Environment(
        trim_blocks=True,
        lstrip_blocks=True
    )

    template = env.from_string(template_content)
    output = template.render(dim=dim)

    # Write the output file
    output_filename = f"../Dimension/Dimension_Impl/DerivedDimensions/{dimension_name}Dimension_Impl.h"
    with open(output_filename, "w") as f:
        f.write(output)

    print(f"Generated {output_filename}")

if __name__ == "__main__":
    main()
