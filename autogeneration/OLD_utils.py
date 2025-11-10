# in Generate.py (or a small helpers module)
from typing import Any, Dict, List, Optional, Tuple, Iterable
from metadata.python_utils.unit_model import (
    FundamentalBlock, DerivedBlock, FactorObject, Rational, SymbolTerm
)

def _unwrap_rational(r: Any) -> Tuple[int, int]:
    # r can be Rational RootModel, list/tuple, or None
    if r is None:
        return (1, 1)
    if hasattr(r, "root"):
        r = r.root
    return (int(r[0]), int(r[1]))

def _unwrap_symbol_term(st: Any) -> Tuple[str, Tuple[int, int]]:
    # st is SymbolTerm RootModel or raw [name, Rational]
    if hasattr(st, "root"):
        st = st.root
    name = str(st[0])
    exp = st[1]
    if hasattr(exp, "root"):
        exp = exp.root
    return name, (int(exp[0]), int(exp[1]))

def _qualify_symbol(name: str) -> str:
    # assume your C++ symbols are in symbols::, and names in JSON already match identifiers
    # If some are already qualified, detect and pass through.
    return name if "::" in name else f"symbols::{name}"

# TODO: Not sure if I actually need this...
def _as_list2(x):
    # Unwrap RootModel[List] with length 2, or raw [a,b]
    if hasattr(x, "root"):
        x = x.root
    if not (isinstance(x, (list, tuple)) and len(x) == 2):
        raise ValueError("expected a 2-item list/tuple")
    return x

# TODO: Remove e10 once ratioExponent is fully working
def normalize_conversion(conv: Any) -> Dict[str, Any]:
    """
    Canonical form (no e10):
      {
        "legacy_number": Optional[float],
        "rnum": int,
        "rden": int,
        "symbols": List[{"name": "symbols::<base>", "num": int, "den": int}],
        "ratioExponents": List[{"baseNum": int, "baseDen": int,
                                "expNum": int,  "expDen": int}],
      }
    """
    # Legacy bare number
    if isinstance(conv, (int, float)):
        return {"legacy_number": float(conv), "rnum": 1, "rden": 1, "symbols": [], "ratioExponents": []}

    # Legacy bare [num, den]
    if isinstance(conv, (list, tuple)) and len(conv) == 2 and isinstance(conv[0], (int, float)):
        rnum, rden = conv
        return {"legacy_number": None, "rnum": int(rnum), "rden": int(rden), "symbols": [], "ratioExponents": []}

    # Pydantic RootModels that can appear at the top level
    if hasattr(conv, "root"):
        root = conv.root
        if isinstance(conv, Rational):
            rnum, rden = _as_list2(root)
            return {"legacy_number": None, "rnum": int(rnum), "rden": int(rden), "symbols": [], "ratioExponents": []}
        if isinstance(conv, Factor1):
            x, name = _as_list2(root)
            symbols = [{"name": _qualify_symbol(str(name)), "num": -1, "den": 1}]
            return {"legacy_number": None, "rnum": int(x), "rden": 1, "symbols": symbols, "ratioExponents": []}

    # FactorObject (Pydantic instance or dict)
    get = (lambda k: getattr(conv, k)) if hasattr(conv, "__dict__") else (lambda k: conv.get(k))

    rnum, rden = 1, 1
    symbols: List[Dict[str, int]] = []
    ratio_exps: List[Dict[str, int]] = []

    # r : [num, den]
    r = get("r")
    if r is not None:
        rn, rd = _as_list2(r)
        rnum, rden = int(rn), int(rd)

    # legacy 'sym' : List[SymbolTerm]
    sym_list = get("sym")
    if sym_list:
        for st in sym_list:
            name, (en, ed) = _unwrap_symbol_term(st)
            symbols.append({"name": _qualify_symbol(name), "num": int(en), "den": int(ed)})

    # ratioExponents : List[RatioExponentTerm], each term.root == [[bn,bd],[en,ed]]
    rex = get("ratioExponents")
    if rex:
        if not isinstance(rex, (list, tuple)):
            raise ValueError("ratioExponents must be a list")
        for term in rex:
            base_pair, exp_pair = _as_list2(term)
            bn, bd = map(int, _as_list2(base_pair))
            en, ed = map(int, _as_list2(exp_pair))
            if bd == 0 or ed == 0:
                raise ValueError("denominators in ratioExponents must be non-zero")
            ratio_exps.append({"baseNum": bn, "baseDen": bd, "expNum": en, "expDen": ed})

    return {
        "legacy_number": None,
        "rnum": rnum,
        "rden": rden,
        "symbols": symbols,
        "ratioExponents": ratio_exps,
    }


def _si_prefix_catalog():
    # name, abbr, power-of-10 exponent
    return [
        {"tag": "pico",  "abbr": "p",  "exp": -12},
        {"tag": "nano",  "abbr": "n",  "exp":  -9},
        {"tag": "micro", "abbr": "u",  "exp":  -6},  # use "u" for μ
        {"tag": "milli", "abbr": "m",  "exp":  -3},
        {"tag": "centi", "abbr": "c",  "exp":  -2},
        {"tag": "deci",  "abbr": "d",  "exp":  -1},
        {"tag": "deca",  "abbr": "da", "exp":   1},
        {"tag": "hecto", "abbr": "h",  "exp":   2},
        {"tag": "kilo",  "abbr": "k",  "exp":   3},
        {"tag": "mega",  "abbr": "M",  "exp":   6},
        {"tag": "giga",  "abbr": "G",  "exp":   9},
        {"tag": "tera",  "abbr": "T",  "exp":  12},
    ]

def build_fundamental_render_items(blocks):
    items = []
    for b in blocks:
        base = str(b.BaseUnit)
        base_unit = b.Units[base]
        # collect base->target conversions
        to_map = {}
        convs = getattr(base_unit, "Conversions_1", None)
        if convs and convs.To:
            for target, factor in convs.To.items():
                to_map[target] = normalize_conversion(factor)

        units = []
        for name, u in b.Units.items():
            with_si = []
            if bool(u.SI_Prefixes):
                # generate prefixed units for THIS unit
                for p in _si_prefix_catalog():
                    with_si.append({
                        "key": f"{p['tag']}_{name}",               # e.g., kilo_meters
                        "Name": f"{p['tag']}{u.Name}",             # e.g., kilometers
                        "Abbreviation": f"{p['abbr']}{u.Abbreviation}",  # e.g., km
                        "exp": int(p["exp"]),
                        "base_key": name,                           # unprefixed source
                    })
            units.append({
                "key": name,
                "Name": u.Name,
                "Abbreviation": u.Abbreviation,
                "SI_Prefixes": bool(u.SI_Prefixes),
                "is_base": (name == base),
                "conv_from_base": to_map.get(name),
                "si_generated": with_si,  # list of generated SI units for this anchor
            })

        items.append({
            "Dimension": str(b.Dimension),
            "HasExtras": bool(b.HasExtras),
            "BaseUnit": base,
            "Units": units,
        })
    return items

def build_derived_render_items(blocks: Iterable[DerivedBlock]) -> List[Dict[str, Any]]:
    """
    Returns a list of render-ready dicts for each derived dimension:
      {
        "Dimension": str,
        "HasExtras": bool,
        "Units": [
           {
             "key": "joules",
             "Exponents": [ {"Unit":"kilo_grams","Num":1,"Den":1}, ... ]
           }, ...
        ],
        "HelperUnits": [
           {
             "key": "calorie_mass",
             "Dim": "mass",
             "Name": "Caloriemass",
             "Abbreviation": "Caloriemass",
             "Conversions": [
                {"To": "grams", **normalized_factor_dict}
             ]
           }, ...
        ]
      }
    """
    out: List[Dict[str, Any]] = []
    for b in blocks:
        units_list: List[Dict[str, Any]] = []
        for unit_key, exps in (b.Units or {}).items():
            units_list.append({
                "key": unit_key,
                "Exponents": [
                    {
                        "Unit": e.Unit,            # string identifier
                        "Num": int(e.Exponent),    # ints in your JSON model today
                        "Den": 1                   # keep Den=1 (rational-ready if you extend later)
                    }
                    for e in exps
                ],
            })

        helper_list: List[Dict[str, Any]] = []
        for hk, hu in (b.HelperUnits or {}).items():
            convs = []
            if hu.Conversions_1 and hu.Conversions_1.To:
                for to_name, factor in hu.Conversions_1.To.items():
                    convs.append({"To": to_name, **normalize_conversion(factor)})
            helper_list.append({
                "key": hk,
                "Dim": str(hu.Dimension),
                "Name": hu.Name,
                "Abbreviation": hu.Abbreviation,
                "Conversions": convs,
            })

        out.append({
            "Dimension": str(b.Dimension),
            "HasExtras": bool(b.HasExtras),
            "Units": units_list,
            "HelperUnits": helper_list,
        })
    return out

def build_derived_impl_items(blocks: Iterable[DerivedBlock]) -> List[Dict[str, Any]]:
    """
    Render-ready dicts for DerivedDimension.impl template.
      {
        "Dimension": str,
        "HasExtras": bool,
        "Definition": [ {"dim": "length", "ExponentNum": 1, "ExponentDen": 1}, ... ]
      }
    """
    out: List[Dict[str, Any]] = []
    for b in blocks:
        defs = []
        for d in (b.Definition or []):
            defs.append({
                "dim": str(d.Dimension),
                "ExponentNum": int(d.Exponent),  # current JSON uses ints
                "ExponentDen": 1,                # future-proof if rationals later
            })
        out.append({
            "Dimension": str(b.Dimension),
            "HasExtras": bool(b.HasExtras),
            "Definition": defs,
        })
    return out

def build_header_context(fundamentals, derived):
    # template uses .keys(), so give it dicts keyed by names
    f = {str(b.Dimension): True for b in fundamentals}
    d = {str(b.Dimension): True for b in derived}
    return {"fundamental_dims": f, "derived_dims": d}