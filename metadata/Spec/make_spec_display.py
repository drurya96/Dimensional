from jinja2 import Environment, FileSystemLoader
import json

# Load your JSON spec
with open('full_spec.json') as f:
    spec = json.load(f)

# Setup Jinja environment
env = Environment(loader=FileSystemLoader('.'))
template = env.get_template('spec_display.template')

# Render
output = template.render(spec=spec)

# Write output
with open('dimensional_spec.html', 'w') as f:
    f.write(output)
