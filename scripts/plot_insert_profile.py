#!/usr/bin/python3

"""hash_table
    Copyright 2023 Debby Nirwan
    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at
       http://www.apache.org/licenses/LICENSE-2.0
    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
"""

import map_module
import hash_table_cpp
import pandas as pd
import plotly.express as px

element_index = []
time = []
lib_name = []

data = {
    "i-th element": element_index,
    "time (us)": time,
    "library name": lib_name
}

n = 1000000
result = map_module.profile_insert(n)
result_hash_table = hash_table_cpp.profile_insert(n)

for i in range(n):
    lib_name.append("Map")
    element_index.append(i)
    time.append(result.map_time.at(i))

    lib_name.append("std::map")
    element_index.append(i)
    time.append(result.std_map_time.at(i))

    lib_name.append("HashTable")
    element_index.append(i)
    time.append(result_hash_table.hash_table_time.at(i))

    lib_name.append("std::unordered_map")
    element_index.append(i)
    time.append(result_hash_table.unordered_map_time.at(i))

df = pd.DataFrame(data)
fig = px.line(df, log_x=False, markers=True, title="Insert Profile - Time",
              x="i-th element", y="time (us)", color="library name")
fig.write_image(file="insert_profile.png", scale=3.0)
