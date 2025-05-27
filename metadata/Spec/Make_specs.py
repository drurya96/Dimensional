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







def main():
   with open('full_spec.json') as f:
      spec = json.load(f)

   make_domains(spec)
   make_contexts(spec)
   #make_behaviors(spec)


if __name__=="__main__":
   main()