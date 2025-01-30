import os
import sys
from jinja2 import Template
from collections import Counter

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

    all_units = set(numerator_list + denominator_list)

    def make_template_list(incoming_list):
        output_list_counter = Counter()
        return [
            f"{unit}{output_list_counter.update([unit]) or output_list_counter[unit]}"
            for unit in incoming_list
        ]

    numerator_template_list = make_template_list(numerator_list)
    denominator_template_list = make_template_list(denominator_list)

    doxygen_tparam_string = "\n".join([f"/// @tparam {param}" for param in numerator_template_list + denominator_template_list])

    # Construct parameters for the template
    template_params = {
        "dimension_name": dimension_name,
        "numerator_template_types": ", ".join([f"typename {unit}" for unit in numerator_template_list]),
        "denominator_template_types": ", ".join([f"typename {unit}" for unit in denominator_template_list]),
        "numerator_template_list_non_numbered": numerator_list,
        "denominator_template_list_non_numbered": denominator_list,
        "numerator_template_typesT": ", ".join([f"typename {unit}" for unit in [f"{unit}T" for unit in numerator_template_list]]),
        "denominator_template_typesT": ", ".join([f"typename {unit}" for unit in [f"{unit}T" for unit in denominator_template_list]]),
        "numerator_types": ", ".join(numerator_template_list),
        "denominator_types": ", ".join(denominator_template_list),
        "numerator_typesT": ", ".join([f"{unit}T" for unit in numerator_template_list]),
        "denominator_typesT": ", ".join([f"{unit}T" for unit in denominator_template_list]),
        "extracted_numerator_types": ", ".join([f"typename std::tuple_element_t<{i}, typename T::NumTuple>" for i in range(len(numerator_template_list))]),
        "extracted_denominator_types": ", ".join([f"typename std::tuple_element_t<{i}, typename T::DenTuple>" for i in range(len(denominator_template_list))]),
        "dimension_checks": " &&\n        ".join([f"std::is_same_v<typename {unit}::Dim, {unit[:-1]}Type>" for unit in numerator_template_list + denominator_template_list]),
        "includes": "\n".join([f"#include \"../../{unit}Dimension.h\"" for unit in all_units]),
        "examples": "e.g., specific units",
        "doxygen_tparam_string": "\n".join([f"/// @tparam {param}" for param in numerator_template_list + denominator_template_list]),
        "numerator_template_list": numerator_template_list,
        "denominator_template_list": denominator_template_list,
        "num_size": len(numerator_template_list),
        "den_size": len(denominator_template_list)
    }

    # Load the template from an external file
    template_path = "DerivedDimension.template"
    with open(template_path, "r") as file:
        template_content = file.read()

    template = Template(template_content)
    output = template.render(**template_params)

    # Write the output file
    output_filename = f"Dimension/Dimension_Impl/DerivedDimensions/{dimension_name}Dimension_Impl.h"
    with open(output_filename, "w") as f:
        f.write(output)

    print(f"Generated {output_filename}")

if __name__ == "__main__":
    main()
