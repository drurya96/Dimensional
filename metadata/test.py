from .unit_model import DimensionalUnitsDimensions
import json


with open('metadata/DerivedUnits.json', "r") as f:
    data = json.load(f)

model = DimensionalUnitsDimensions.model_validate(data)

print(model)