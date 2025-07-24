import json


def make_domains(spec):
    # Extract only domain-level data
    data = {
        "domains": [
            {
                "id": cap["id"],
                "name": cap["name"],
                "description": cap["description"]
            }
            for cap in spec.get("domains", [])
        ]
    }

    with open('domain_spec.json', 'w') as f:
        json.dump(data, f, indent=2)


def make_contexts(spec):
    # Extract domain and context level, discard rules
    data = {
        "domains": [
            {
                "id": cap["id"],
                "name": cap["name"],
                "description": cap["description"],
                "contexts": [
                    {
                        "id": mod["id"],
                        "name": mod["name"],
                        "description": mod["description"]
                    }
                    for mod in cap.get("contexts", [])
                ]
            }
            for cap in spec.get("domains", [])
        ]
    }

    with open('context_spec.json', 'w') as f:
        json.dump(data, f, indent=2)


def make_behaviors(spec):
    # Extract domain -> context -> rules, discard contract.clauses
    data = {
        "domains": [
            {
                "id": cap["id"],
                "name": cap["name"],
                "description": cap["description"],
                "contexts": [
                    {
                        "id": mod["id"],
                        "name": mod["name"],
                        "description": mod["description"],
                        "rules": [
                            {
                                "id": con["id"],
                                "name": con["name"],
                                "behavior": con.get("behavior"),
                                "kind": con.get("kind")
                            }
                            for con in mod.get("rules", [])
                        ]
                    }
                    for mod in cap.get("contexts", [])
                ]
            }
            for cap in spec.get("domains", [])
        ]
    }

    with open('behavior_spec.json', 'w') as f:
        json.dump(data, f, indent=2)


def print_ids(spec):
    for domain in spec.get("domains", []):
         for context in domain.get("contexts", []):
             for rule in context.get("rules", []):
                 print(f"{domain.get("id")}:{context.get("id")}:{rule.get("id")}")



def write_test(file_handle, domain, context, rule, kind, description):
    
    if kind == "positive_compile_time":
        error = "false"
    elif kind == "negative_compile_time":
        error = "true"
    else:
        raise("Kind not appropriate")

    file_handle.write(f"struct {domain}_{context}_{rule} {{\n")
    file_handle.write(f"   static constexpr const char* id = \"{domain}:{context}:{rule}\";\n")
    file_handle.write(f"   static constexpr bool expect_error = {error};\n")
    file_handle.write(f"   static constexpr const char* description = \"{description}\";\n\n")
    file_handle.write("   template<typename = void>\n")
    file_handle.write("      static void run() {\n\n")
    file_handle.write("   }\n")
    file_handle.write("};\n\n")

def write_header(filename, domain):

    with open(filename, 'w') as f:
        f.write(f"#ifndef DIMENSIONAL_COMPILE_TEST_{domain.get("id")}\n")
        f.write(f"#define DIMENSIONAL_COMPILE_TEST_{domain.get("id")}\n\n")

        f.write("#include \"length_dimension.h\"\n")
        f.write("#include \"timespan_dimension.h\"\n\n")

        f.write("using namespace dimension;\n\n")
        for context in domain.get("contexts", []):
            for rule in context.get("rules", []):
                write_test(f, domain.get("id"), context.get("id"), rule.get("id"), rule.get("kind"), rule.get("behaviors")[0])

        f.write(f"#endif // DIMENSIONAL_COMPILE_TEST_{domain.get("id")}\n")

def write_headers(spec):
    for domain in spec.get("domains", []):
        filename = f"CompileTests/compile_test_{domain.get("id")}.h"
        write_header(filename, domain)


def main():
   with open('full_spec.json') as f:
      spec = json.load(f)

   #make_domains(spec)
   #make_contexts(spec)
   #make_behaviors(spec)
   #print_ids(spec)
   write_headers(spec)

   


if __name__=="__main__":
   main()