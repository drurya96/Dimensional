from dataclasses import dataclass, field
from typing import List, Dict
import json
from pathlib import Path


@dataclass
class Conversions:
    """
    Represents a set of conversions between a unit and other units.
    """
    From: Dict[str, float] = field(default_factory={})
    To: Dict[str, float] = field(default_factory={})

    def __str__(self) -> str:
        return f"Conversion(From={self.From}, To={self.To})"
    
    @classmethod
    def parse(cls, obj: Dict) -> "Conversions":
        """Parse a Conversions object from a JSON dictionary"""
        return cls(
            From=obj.get("From", {}),
            To=obj.get("To", {})
        )

@dataclass
class Unit:
    """
    Represents a unit of measurement, optionally with SI prefixes and a set of conversions.
    """
    dim: str
    name: str
    abbreviation: str
    si_prefixes: bool = field(default=False)
    conversions: Conversions = field(default_factory=Conversions)

    def __str__(self) -> str:
        return f"Unit(name='{self.name}', abbreviation='{self.abbreviation}', si_prefixes={self.si_prefixes}, conversion={self.conversion})"

    @classmethod
    def parse(cls, obj: Dict) -> "Unit":
        """Parse a Unit object from a JSON dictionary."""
        return cls(
            dim=obj.get("Dimension", ""),
            name=obj.get("Name"),
            abbreviation=obj.get("Abbreviation"),
            si_prefixes=obj.get("SI_Prefixes", False),
            conversions=Conversions.parse(obj.get("Conversions", {}))
        )

    @classmethod
    def parse_units(cls, obj: Dict) -> Dict[str, "Unit"]:
        """Parse a dictionary of units from a JSON dictionary."""
        return {name: cls.parse(value) for name, value in obj.items()}

@dataclass
class UnitExponent:
    """
    Represents a unit raised to a rational exponent (e.g., meters^2, seconds^-1).
    """
    unit: str
    exponent_num: int
    exponent_den: int = field(default = 1)

    def __str__(self) -> str:
        return f"UnitExponent(unit='{self.unit}', exponent={self.exponent_num}/{self.exponent_den})"

    @classmethod
    def parse(cls, obj: Dict) -> "UnitExponent":
        """Parse a UnitExponent object from a JSON dictionary."""
        exponent = obj.get("Exponent")
        if isinstance(exponent, int):
            return cls(
                unit=obj.get("Unit"),
                exponent_num=exponent,
                exponent_den=1
            )
        else:
            return cls(
                unit=obj.get("Unit"),
                exponent_num=exponent[0],
                exponent_den=exponent[1]
            )

@dataclass
class DimensionExponent:
    """
    Represents a dimension raised to a rational exponent (e.g., Length^2, Time^-1).
    """
    dim: str
    exponent_num: int
    exponent_den: int = 1

    def __str__(self) -> str:
        return f"DimensionExponent(dim='{self.dim}', exponent={self.exponent_num}/{self.exponent_den})"

    @classmethod
    def parse(cls, obj: Dict) -> "DimensionExponent":
        """Parse a DimensionExponent object from a JSON dictionary."""
        exponent = obj.get("Exponent")
        if isinstance(exponent, int):
            return cls(
                dim=obj.get("Dimension"),
                exponent_num=exponent,
                exponent_den=1
            )
        else:
            return cls(
                dim=obj.get("Dimension"),
                exponent_num=exponent[0],
                exponent_den=exponent[1]
            )

@dataclass
class Dimension:
    """
    Base class for all dimensions (fundamental and derived).
    """
    name: str
    is_fundamental: bool
    has_extras: bool

    def __str__(self) -> str:
        return f"Dimension(name='{self.name}', fundamental={self.is_fundamental})"

    @classmethod
    def parse(cls, obj: Dict) -> "Dimension":
        """
        Dispatch parsing based on whether the object represents a fundamental or derived dimension.
        """
        if obj.get("Fundamental"):
            return FundamentalDimension.parse(obj)
        else:
            return DerivedDimension.parse(obj)

@dataclass
class FundamentalDimension(Dimension):
    """
    Represents a fundamental physical dimension (e.g., Length, Mass, Time).
    """
    base_unit: str
    units: Dict[str, Unit]

    def __str__(self) -> str:
        return f"FundamentalDimension(name='{self.name}', base_unit='{self.base_unit}', units=[{', '.join(self.units.keys())}])"

    @classmethod
    def parse(cls, obj: Dict) -> "FundamentalDimension":
        """Parse a FundamentalDimension object from a JSON dictionary."""
        return cls(
            name=obj.get("Dimension"),
            is_fundamental = True,
            has_extras=obj.get("HasExtras", False),
            base_unit=obj.get("BaseUnit"),
            units=Unit.parse_units(obj.get("Units", {}))
        )

@dataclass
class DerivedDimension(Dimension):
    """
    Represents a derived physical dimension (e.g., Speed = Length/Time).
    """
    definition: List[DimensionExponent]
    units: Dict[str, List[UnitExponent]]
    helper_units: Dict[str, Unit]

    def __str__(self) -> str:
        return f"DerivedDimension(name='{self.name}', definition=[{', '.join(str(d) for d in self.definition)}], units=[{', '.join(self.units.keys())}])"

    @classmethod
    def parse(cls, obj: Dict) -> "DerivedDimension":
        """Parse a DerivedDimension object from a JSON dictionary."""
        return cls(
            name=obj.get("Dimension"),
            is_fundamental = False,
            has_extras=obj.get("HasExtras", False),
            definition=[DimensionExponent.parse(d) for d in obj.get("Definition", [])],
            units={
                unit_name: [UnitExponent.parse(e) for e in exponents]
                for unit_name, exponents in obj.get("Units", {}).items()
            },
            helper_units=Unit.parse_units(obj.get("HelperUnits", {}))
        )

@dataclass
class AllDimension:
    """
    Aggregates all parsed dimensions, both fundamental and derived, for a complete unit system.
    """
    fundamental_dims: Dict[str, FundamentalDimension]
    derived_dims: Dict[str, DerivedDimension]

    def __str__(self) -> str:
        return f"AllDimension(fundamental={list(self.fundamental_dims.keys())}, derived={list(self.derived_dims.keys())})"

    @classmethod
    def parse(cls, obj: List[Dict]) -> "AllDimension":
        """Parse all dimensions from a JSON list of dictionaries."""
        fundamental: Dict[str, FundamentalDimension] = {}
        derived: Dict[str, DerivedDimension] = {}
        for item in obj:
            dim = Dimension.parse(item)
            if dim.is_fundamental:
                fundamental[dim.name] = dim
            else:
                derived[dim.name] = dim
        return cls(fundamental, derived)








def main():
    # Load two JSON files
    file1 = Path("../FundamentalUnits.json")
    file2 = Path("../DerivedUnits.json")

    with file1.open() as f:
        fundamental_data = json.load(f)

    with file2.open() as f:
        derived_data = json.load(f)

    # Combine the two JSON lists
    combined_data = fundamental_data + derived_data
    
    # Parse into AllDimension
    all_dimensions = AllDimension.parse(combined_data)

    # Print summary
    print(all_dimensions)

if __name__ == "__main__":
    main()
