# utils.py
from typing import Any, Dict, List, Tuple, Iterable
from metadata.python_utils.unit_model import (
    FundamentalBlock, DerivedBlock,
    FactorObject, Rational, SymbolTerm, RatioTerm,
    HelperUnitDef,
)

# ---------- low-level unwraps ----------

def _unwrap_rational_obj(r: Any) -> Tuple[int, int]:
    """
    Accepts None | Rational | dict {'num':..., 'den':...}
    Returns (num, den) with den defaulted to 1.
    """
    if r is None:
        return (1, 1)
    if isinstance(r, Rational):
        num = int(r.num)
        den = 1 if r.den is None else int(r.den)
        return (num, den)
    # dict-like
    num = int(r.get("num"))
    den = int(r.get("den", 1))
    return (num, den)

def _qualify_symbol(name: str) -> str:
    return name if "::" in name else f"symbols::{name}"

def _unwrap_symbol_term_obj(st: Any) -> Tuple[str, Tuple[int, int]]:
    """
    Accepts SymbolTerm | dict {'name':..., 'exp': Rational?}
    Returns (qualified_name, (exp_num, exp_den)).
    """
    if isinstance(st, SymbolTerm):
        name = st.name
        en, ed = _unwrap_rational_obj(st.exp)
    else:
        name = st["name"]
        en, ed = _unwrap_rational_obj(st.get("exp"))
    return _qualify_symbol(str(name)), (int(en), int(ed))

def _unwrap_ratio_term_obj(rt: Any) -> Tuple[Tuple[int, int], Tuple[int, int]]:
    """
    Accepts RatioTerm | dict {'base': Rational, 'exp': Rational?}
    Returns ((base_num, base_den), (exp_num, exp_den)).
    """
    if isinstance(rt, RatioTerm):
        bn, bd = _unwrap_rational_obj(rt.base)
        en, ed = _unwrap_rational_obj(rt.exp)
    else:
        bn, bd = _unwrap_rational_obj(rt["base"])
        en, ed = _unwrap_rational_obj(rt.get("exp"))
    return (int(bn), int(bd)), (int(en), int(ed))

# ---------- public normalizer used by Jinja contexts ----------

def normalize_conversion(conv: Any) -> Dict[str, Any]:
    """
    Canonical dict for templates (no raw scalar; keep rnum/rden=1 as a stable shim):
      {
        "rnum": 1,
        "rden": 1,
        "symbols":       [ {"name":"symbols::<id>","num":en,"den":ed}, ... ],
        "ratioExponents":[ {"baseNum":bn,"baseDen":bd,"expNum":en,"expDen":ed}, ... ],
      }
    """
    # FactorObject (Pydantic or plain dict per new schema)
    if isinstance(conv, FactorObject):
        sym_list = conv.sym or []
        rex_list = conv.ratio or []
    else:
        # dict-like
        sym_list = (conv.get("sym") or [])
        rex_list = (conv.get("ratio") or [])

    symbols: List[Dict[str, int]] = []
    ratio_exps: List[Dict[str, int]] = []

    for st in sym_list:
        name, (en, ed) = _unwrap_symbol_term_obj(st)
        symbols.append({"name": name, "num": en, "den": ed})

    for rt in rex_list:
        (bn, bd), (en, ed) = _unwrap_ratio_term_obj(rt)
        if bd == 0 or ed == 0:
            raise ValueError("denominators in ratio terms must be non-zero")
        ratio_exps.append({"baseNum": bn, "baseDen": bd, "expNum": en, "expDen": ed})

    return {
        "rnum": 1,
        "rden": 1,
        "symbols": symbols,
        "ratioExponents": ratio_exps,
    }

# ---------- SI prefixes (unchanged policy) ----------

def _si_prefix_catalog():
    # name, abbr, power-of-10 exponent
    return [
        {"tag": "pico",  "abbr": "p",  "exp": -12},
        {"tag": "nano",  "abbr": "n",  "exp":  -9},
        {"tag": "micro", "abbr": "u",  "exp":  -6},  # using 'u' for μ in identifiers
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

# ---------- render item builders (restored & updated) ----------

def build_fundamental_render_items(blocks: Iterable[FundamentalBlock]) -> List[Dict[str, Any]]:
    """
    Produces a list of dicts, each:
      {
        "Dimension": str,
        "HasExtras": bool,
        "BaseUnit":  str,
        "Units": [
           {
             "key": str,
             "Name": str,
             "Abbreviation": str,
             "SI_Prefixes": bool,
             "is_base": bool,
             "conv_from_base": normalized_factor_dict | None,
             "si_generated": [ {generated SI unit info} ]
           }, ...
        ]
      }
    """
    items: List[Dict[str, Any]] = []
    for b in blocks:
        base = str(b.BaseUnit)
        base_unit = b.Units[base]

        # Gather base -> target conversions from the base unit's "To" map
        to_map: Dict[str, Dict[str, Any]] = {}
        convs = getattr(base_unit, "Conversions_1", None)
        if convs and convs.To:
            for target, factor in convs.To.items():
                to_map[target] = normalize_conversion(factor)

        units = []
        for name, u in b.Units.items():
            # SI variants for THIS unit (based on its own SI_Prefixes flag)
            with_si = []
            if bool(u.SI_Prefixes):
                for p in _si_prefix_catalog():
                    with_si.append({
                        "key": f"{p['tag']}_{name}",
                        "Name": f"{p['tag']}{u.Name}",
                        "Abbreviation": f"{p['abbr']}{u.Abbreviation}",
                        "exp": int(p["exp"]),
                        "base_key": name,
                    })

            units.append({
                "key": name,
                "Name": u.Name,
                "Abbreviation": u.Abbreviation,
                "SI_Prefixes": bool(u.SI_Prefixes),
                "is_base": (name == base),
                "conv_from_base": to_map.get(name),
                "si_generated": with_si,
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
    Returns list:
      {
        "Dimension": str,
        "HasExtras": bool,
        "Units": [
          { "key": unit_key, "Exponents": [ {"Unit": "...","Num":int,"Den":1}, ... ] }
        ],
        "HelperUnits": [
          {
            "key": hk, "Dim": str, "Name": str, "Abbreviation": str,
            "Conversions": [ {"To": to_name, **normalized_factor_dict}, ... ]
          }
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
                    {"Unit": e.Unit, "Num": int(e.Exponent), "Den": 1}
                    for e in exps
                ],
            })

        helper_list: List[Dict[str, Any]] = []
        for hk, hu in (b.HelperUnits or {}).items():
            convs = []
            c = hu.Conversions_1
            if c and c.To:
                for to_name, factor in c.To.items():
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
    For DerivedDimension.impl:
      {
        "Dimension": str,
        "HasExtras": bool,
        "Definition": [ {"dim": str, "ExponentNum": int, "ExponentDen": 1}, ... ]
      }
    """
    out: List[Dict[str, Any]] = []
    for b in blocks:
        defs = []
        for d in (b.Definition or []):
            defs.append({
                "dim": str(d.Dimension),
                "ExponentNum": int(d.Exponent),
                "ExponentDen": 1,
            })
        out.append({
            "Dimension": str(b.Dimension),
            "HasExtras": bool(b.HasExtras),
            "Definition": defs,
        })
    return out

def build_header_context(fundamentals: Iterable[FundamentalBlock],
                         derived: Iterable[DerivedBlock]) -> Dict[str, Dict[str, bool]]:
    """
    Used by the all-dimensions header template.
      returns { "fundamental_dims": {dim: True,...}, "derived_dims": {dim: True,...} }
    """
    f = {str(b.Dimension): True for b in fundamentals}
    d = {str(b.Dimension): True for b in derived}
    return {"fundamental_dims": f, "derived_dims": d}
