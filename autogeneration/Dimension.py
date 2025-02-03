from collections import Counter

class dimension_list:
   def __init__(self):
      self.list = None
      self.list_numbered = None
      self.unique_dims = None
      self.size = None

   def make_numbered_list(self):
      output_list_counter = Counter()
      self.list_numbered = [
         f"{unit}{output_list_counter.update([unit]) or output_list_counter[unit]}"
         for unit in self.list
      ]
    
   def set_list(self, incoming_list):
      if incoming_list is not None:
         self.unique_dims = []
         self.list = incoming_list
         [self.unique_dims.append(x) for x in self.list if x not in self.unique_dims]
         self.make_numbered_list()
         self.size = len(self.list)

class dimension:
   def __init__(self):
      self.name = None
      self.fundamental = None
      self.primary = None
      self.num = dimension_list()
      self.den = dimension_list()

   def init_from_json(self, obj):
      self.name = obj.get("Dimension", None)
      self.fundamental = obj.get("Fundamental", None)
      self.primary = obj.get("BaseUnit", None)
      self.set_num_list(obj.get("Numerator", None))
      self.set_den_list(obj.get("Denominator", None))

   def set_num_list(self, incoming_list):
      self.num.set_list(incoming_list)

   def set_den_list(self, incoming_list):
      self.den.set_list(incoming_list)